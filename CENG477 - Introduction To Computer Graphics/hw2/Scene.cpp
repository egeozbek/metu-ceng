#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <fstream>
#include <cmath>

#include "Scene.h"
#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "tinyxml2.h"
#include "Helpers.h"

using namespace tinyxml2;
using namespace std;

void Scene::forwardRenderingPipeline(Camera *camera)
{
	//for each model, do modelling transforms
	for(int i = 0 ; i < models.size() ; i++ ){
		Model * currentModel = models[i];
		//copyVertices because at each for camera loop , we overwrite it , vertices are given w.r.t their ids
		vector< Vec4 > copyVertices;
		double one = 1.0;
		for(int i = 0 ; i < vertices.size();i++){
			Vec4 copyVertex = Vec4(vertices[i]->x,vertices[i]->y,vertices[i]->z,one,vertices[i]->colorId);
			copyVertices.push_back(copyVertex);
		}
		for(int j = 0 ; j < currentModel->numberOfTransformations ; j++){
			char transformationType = currentModel->transformationTypes[j];
			//cout <<"Transformation Ids"<<endl;
			int transformationId = currentModel->transformationIds[j]-1; // transformation id with 1 is kept at vector index 0
			//for each transformation kind
			set <int, greater <int> > verticesMarkedForTransformation;
			//since a vertex can be in many faces , we only need to transform it once
			for(int k = 0 ; k < currentModel->numberOfTriangles ; k++){
				//for each vertex in triangle(here they are indexed starting from 1 )
				verticesMarkedForTransformation.insert(currentModel->triangles[k].getFirstVertexId());
				verticesMarkedForTransformation.insert(currentModel->triangles[k].getSecondVertexId());
				verticesMarkedForTransformation.insert(currentModel->triangles[k].getThirdVertexId());
			}
			doModellingTransformation(transformationType,transformationId,verticesMarkedForTransformation,copyVertices);
		}
				//transform vertices from world coordinates to camera coordinates
		doViewingTransformation(camera,copyVertices);
		//after viewing transformations , do projections
		
		if(projectionType == 0){
			//cout<<"Projection type orthographic"<<endl;
			doOrthographicProjection(camera,copyVertices);
		}
		else if(projectionType == 1){
			//cout<<"Projection type perspective"<<endl;
			doPerspectiveProjection(camera,copyVertices);
		}
		//with perspective divide , make sure each vertex has w=1.0 (important for clipping)
		doPerspectiveDivide(copyVertices);

		//if wireframe or closed shape

		if(currentModel -> type == 0){ 
			// if model is wireframe , clip it
			for(int k = 0 ; k < currentModel->numberOfTriangles ; k++){
				//bad code incoming :(
				bool drawCurrentTriangle = true;
				//for each vertex in triangle (here they are indexed starting from 1 )
				int firstVertexIndex = currentModel->triangles[k].getFirstVertexId()-1;
				int secondVertexIndex = currentModel->triangles[k].getSecondVertexId()-1;
				int thirdVertexIndex = currentModel->triangles[k].getThirdVertexId()-1;

				//if culling enabled and triangle is backfacing dont draw it
				if(cullingEnabled && !checkBackfaceCulling(camera,copyVertices,firstVertexIndex,secondVertexIndex,thirdVertexIndex)){
					drawCurrentTriangle=false;
				}

				if(drawCurrentTriangle){
					//just clip them
					Vec3 minVertex = Vec3(-1.0,-1.0,-1.0,-1);
					Vec3 maxVertex = Vec3(1.0,1.0,1.0,-1);
					pair<Vec4,Vec4> firstClipped = liangBarskyImplementation(minVertex,maxVertex,copyVertices,firstVertexIndex,secondVertexIndex);
					pair<Vec4,Vec4> secondClipped = liangBarskyImplementation(minVertex,maxVertex,copyVertices,secondVertexIndex,thirdVertexIndex);
					pair<Vec4,Vec4> thirdClipped = liangBarskyImplementation(minVertex,maxVertex,copyVertices,thirdVertexIndex,firstVertexIndex);
					//get screen coordinates
					doPairViewportTransformation(camera,firstClipped);
					doPairViewportTransformation(camera,secondClipped);
					doPairViewportTransformation(camera,thirdClipped);
					//print them using line interpolation(midpoint algorithm)
					lineRasterizer(camera,firstClipped);
					lineRasterizer(camera,secondClipped);
					lineRasterizer(camera,thirdClipped);
				}
			}
		}
		else if (currentModel->type == 1){
			for(int k = 0 ; k < currentModel->numberOfTriangles ; k++){
				//bad code incoming
				bool drawCurrentTriangle = true;
				//for each vertex in triangle(here they are indexed starting from 1 )
				int firstVertexIndex = currentModel->triangles[k].getFirstVertexId()-1;
				int secondVertexIndex = currentModel->triangles[k].getSecondVertexId()-1;
				int thirdVertexIndex = currentModel->triangles[k].getThirdVertexId()-1;
				//get screen coordinates
				tuple<Vec4,Vec4,Vec4> triangleTransformed = doTriangleViewportTransformation(camera,copyVertices, firstVertexIndex , secondVertexIndex,thirdVertexIndex);
				
				//if culling enabled and triangle is backfacing dont draw it
				if(cullingEnabled && !checkBackfaceCulling(camera,copyVertices,firstVertexIndex,secondVertexIndex,thirdVertexIndex)){
					drawCurrentTriangle=false;
				}
				if(drawCurrentTriangle){
					//print them using barycentric interpolation
					barycentricRasterizer(camera,triangleTransformed);
				}
			}
		}
		else{
			cout<<"Undefined model type"<<endl;
			exit(-1);
		}
		//end of models[i]
	}
}



void Scene::doPerspectiveDivide(vector<Vec4> & copyVertices){
	for(int i = 0 ; i < copyVertices.size();i++){
		if(abs(copyVertices[i].t - 1.0)>EPSILON){
			//cout<<"Perspective divide necessary : "<<copyVertices[i].t<<endl;
			copyVertices[i].x /= copyVertices[i].t;
			copyVertices[i].y /= copyVertices[i].t;
			copyVertices[i].z /= copyVertices[i].t;
			copyVertices[i].t /= copyVertices[i].t;
		}
	}
}

/*
 * Return true if given triangle should be culled	
 */
bool Scene :: checkBackfaceCulling(Camera * camera , vector<Vec4> & copyVertices, int firstPointIndex, int secondPointIndex , int thirdPointIndex){
	Vec4 & firstPoint = copyVertices[firstPointIndex];
	Vec4 & secondPoint = copyVertices[secondPointIndex];
	Vec4 & thirdPoint = copyVertices[thirdPointIndex];
	double midX = (firstPoint.x + secondPoint.x + thirdPoint.x)/3;
	double midY = (firstPoint.y + secondPoint.y + thirdPoint.y)/3;
	double midZ = (firstPoint.z + secondPoint.z + thirdPoint.z)/3;
	//as camera is located in middle of box 0,0,0
	Vec3 cameraToMidPointVec = Vec3(midX,midY,midZ,-1);
	//
	Vec3 firstToSecond = subtractVec3(vec4ToVec3(firstPoint),vec4ToVec3(secondPoint));
	Vec3 firstToThird = subtractVec3(vec4ToVec3(firstPoint),vec4ToVec3(thirdPoint));
	Vec3 triangleNormal = crossProductVec3(firstToSecond,firstToThird);
	double dotProduct = dotProductVec3(cameraToMidPointVec,triangleNormal);
	if(dotProduct<=0){
		return false;
	}
	else{
		return true;
	}
}


void Scene :: markPixelwithColor(Camera * camera, int x , int y , Color & colorInfo){
	//this line saves you from segmentation fault
	if(x < 0 || x >= camera->horRes || y < 0 || y >= camera->verRes){
		return;
	}
	this->image[x][y].r = makeBetweenZeroAnd255(colorInfo.r);
	this->image[x][y].g = makeBetweenZeroAnd255(colorInfo.g);
	this->image[x][y].b = makeBetweenZeroAnd255(colorInfo.b);
}

void Scene::lineRasterizer(Camera * camera ,pair<Vec4,Vec4> & clippedPair ){
	//check slope
	Vec4 firstPoint = clippedPair.first;
	Vec4 secondPoint = clippedPair.second;
	//cout << "Line rasterizer called"<<endl;
	//cout << "First Point coordinates" <<firstPoint<<endl;
	//cout << "Second Point coordinates" <<secondPoint<<endl;

	Color * firstPointColorPtr = colorsOfVertices[firstPoint.colorId -1 ]; 
	Color * secondPointColorPtr = colorsOfVertices[secondPoint.colorId -1 ];

	Color currentColor = Color(0.0,0.0,0.0);
	//TODO since double == doesnt make sense , use ABS(f-s)
	//if vertical line
	if(firstPoint.x == secondPoint.x){
		int currentY = min(firstPoint.y , secondPoint.y);
		int endingY = max(firstPoint.y , secondPoint.y);
		int differenceSteps = abs((int)firstPoint.y - (int)secondPoint.y);
		Color differenceColorPerStep;
		if(firstPoint.y < secondPoint.y){
			copyColorValues(&currentColor , firstPointColorPtr);
			differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
		}
		else{
			copyColorValues(&currentColor , secondPointColorPtr);
			differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
		}
		cout<<"Vertical line , startingY : "<<currentY << " ending Y : "<<endingY<<endl;

		while(currentY < endingY){
			markPixelwithColor(camera ,firstPoint.x,currentY,currentColor);
			addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
			currentY++;
		}
	}
	//if horizontal line
	else if(firstPoint.y == secondPoint.y){
		int currentX = min(firstPoint.x , secondPoint.x);
		int endingX = max(firstPoint.x , secondPoint.x);
		int differenceSteps = abs((int)firstPoint.x - (int)secondPoint.x);
		cout<<"Horizontal line , startingX : "<<currentX << " ending X : "<<endingX<<endl;
		//Calculate color difference
		Color differenceColorPerStep;
		if(firstPoint.x < secondPoint.x){
			copyColorValues(&currentColor , firstPointColorPtr);
			differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
		}
		else{
			copyColorValues(&currentColor , secondPointColorPtr);
			differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
		}
		while(currentX < endingX){
			markPixelwithColor(camera,currentX,firstPoint.y,currentColor);
			addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
			currentX++;
		}
	}
	//slope is 1 or -1
	else if( abs(firstPoint.x - secondPoint.x) == abs(firstPoint.y - secondPoint.y)){
		int currentX = firstPoint.x;
		int currentY = firstPoint.y;
		int differenceSteps = abs((int)firstPoint.x - (int)secondPoint.x);
		cout << "Slope 1 or -1 , startingX : "<< currentX << "startingY : "<<currentY <<endl;
		cout << "Slope 1 or -1 , endingX : "<< secondPoint.x << "endingY : "<<secondPoint.y <<endl;
		int xSign = 1;
		int ySign = 1;
		//signbit returns true if negative
		//isXSlopeNegative
		if(signbit(secondPoint.x - firstPoint.x)){
			cout << "x sign is -1"<<endl;
			xSign = -1;
		}
		//isYSlopeNegative
		if(signbit(secondPoint.y - firstPoint.y)){
			cout << "y sign is -1"<<endl;
			ySign = -1;
		}
		//Calculate color difference
		Color differenceColorPerStep;
		if(firstPoint.x < secondPoint.x){
			copyColorValues(&currentColor , firstPointColorPtr);
			differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
		}
		else{
			copyColorValues(&currentColor , secondPointColorPtr);
			differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
		}
		do{
			markPixelwithColor(camera,currentX,currentY,currentColor);
			currentX += xSign;
			currentY += ySign;
			addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
		}
		while(currentX != secondPoint.x && currentY != secondPoint.y );
		markPixelwithColor(camera,currentX,currentY,currentColor);
	}
	//for other 
	else{
		//first calculate the slope
		int toE, toNE, toN, toW, toNW;
		int differenceSteps;
		//Calculate color difference
		Color differenceColorPerStep;

		double slope = (secondPoint.y - firstPoint.y) / (secondPoint.x - firstPoint.x);
		//cout << "slope is : "<<slope<<endl;
		int dx = abs(firstPoint.x - secondPoint.x);
		int dy = abs(firstPoint.y - secondPoint.y);
		//cout << "dx is : "<<dx << " dy is : "<<dy<<endl;

		if(slope > 0 && slope < 1){
			//first quadrant
			//cout << "Slope is between 0 and 1"<<endl;
			differenceSteps= abs((int)firstPoint.x - (int)secondPoint.x);
			if(firstPoint.x < secondPoint.x){
				copyColorValues(&currentColor , firstPointColorPtr);
				differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
			}
			else{
				copyColorValues(&currentColor , secondPointColorPtr);
				differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
			}
			int currentX = min(firstPoint.x , secondPoint.x);
			int currentY = min(firstPoint.y , secondPoint.y);
			int endingX = max(firstPoint.x , secondPoint.x);
			toE = 2 * dy;
			toNE = 2*(dy-dx);
			int slopeChange = 2*dy - dx;
			markPixelwithColor(camera,currentX,currentY,currentColor);
			while(currentX < endingX){
				if(slopeChange <= 0){
					slopeChange += toE;
				}
				else{
					slopeChange += toNE;
					currentY++;
				}
				currentX++;
				addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
				markPixelwithColor(camera,currentX,currentY,currentColor);
			}
		}
		else if(slope > 1){
			//cout << "Slope is greater than 1"<<endl;
			differenceSteps= abs((int)firstPoint.y - (int)secondPoint.y);
			if(firstPoint.y < secondPoint.y){
				copyColorValues(&currentColor , firstPointColorPtr);
				differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
			}
			else{
				copyColorValues(&currentColor , secondPointColorPtr);
				differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
			}

			int currentX = min(firstPoint.x , secondPoint.x);
			int currentY = min(firstPoint.y , secondPoint.y);
			int endingY = max(firstPoint.y , secondPoint.y);
			toN = 2 * dx;
			toNE = 2 * (dx-dy);
			int slopeChange = 2*dx - dy;
			markPixelwithColor(camera,currentX,currentY,currentColor);
			while(currentY < endingY){
				if(slopeChange <= 0){
					slopeChange += toN;
				}
				else{
					slopeChange += toNE;
					currentX++;
				}
				currentY++;
				addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
				markPixelwithColor(camera,currentX,currentY,currentColor);
			}
		}
		else if(slope < 0 && slope > -1){
			differenceSteps= abs((int)firstPoint.x - (int)secondPoint.x);
			if(firstPoint.x < secondPoint.x){
				copyColorValues(&currentColor , firstPointColorPtr);
				differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
			}
			else{
				copyColorValues(&currentColor , secondPointColorPtr);
				differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
			}
			//cout << "Slope is between 0 and -1"<<endl;
			int currentX = min(firstPoint.x , secondPoint.x);
			int currentY = max(firstPoint.y , secondPoint.y);
			int endingX = max(firstPoint.x , secondPoint.x);
			int slopeChange = 2* dx - dy;
			toW = 2 * dy;
			toNW = 2 * (dy-dx);
			markPixelwithColor(camera,currentX,currentY,currentColor);
			while(currentX < endingX){
				if(slopeChange <= 0){
					slopeChange += toW;
				}
				else{
					slopeChange += toNW;
					currentY--;
				}
				currentX++;
				addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
				markPixelwithColor(camera,currentX,currentY,currentColor);
			}
		}
		else{
			differenceSteps= abs((int)firstPoint.y - (int)secondPoint.y);
			if(firstPoint.y < secondPoint.y){
				copyColorValues(&currentColor , firstPointColorPtr);
				differenceColorPerStep = calculateColorStep(secondPointColorPtr , firstPointColorPtr,differenceSteps);
			}
			else{
				copyColorValues(&currentColor , secondPointColorPtr);
				differenceColorPerStep = calculateColorStep(firstPointColorPtr,secondPointColorPtr,differenceSteps);
			}
			//if slope is shallower than -1
			//cout << "Slope is lesser than -1"<<endl;
			int currentX = max(firstPoint.x , secondPoint.x);
			int currentY = min(firstPoint.y , secondPoint.y);
			int endingY = max(firstPoint.y , secondPoint.y);
			//cout << "StartingX : "<< currentX << "startingY : "<<currentY <<endl;
			int slopeChange = 2*dy - dx;
			toN = 2 * dx ;
			toNW = 2 * (dx - dy);
			markPixelwithColor(camera,currentX,currentY,currentColor);
			while( currentY < endingY){
				//cout<<"Current slope change is "<<slopeChange<<endl;
				if(slopeChange <= 0){
					slopeChange += toN;
				}
				else{
					slopeChange += toNW;
					currentX--;
				}
				currentY++;
				addTwoColors(&currentColor,&currentColor,&differenceColorPerStep);
				markPixelwithColor(camera,currentX,currentY,currentColor);
			}
		}
	}
}
void Scene::barycentricRasterizer(Camera * camera ,tuple<Vec4,Vec4,Vec4> & triangleTuple){
	//cout<<"Enters barycentric rasterizer"<<endl;
	Vec4 firstPoint = get<0> (triangleTuple);
	Vec4 secondPoint = get<1> (triangleTuple);
	Vec4 thirdPoint = get<2> (triangleTuple);

	//bad code , couldn't be bothered to fix it

	vector<double> xValues {firstPoint.x,secondPoint.x,thirdPoint.x};
	vector<double> yValues {firstPoint.y,secondPoint.y,thirdPoint.y};

	vector<double>::iterator min_x_result = min_element(xValues.begin(), xValues.end());
	vector<double>::iterator max_x_result = max_element(xValues.begin(), xValues.end());

	int min_x_index = distance(xValues.begin(), min_x_result);
	int max_x_index = distance(xValues.begin(), max_x_result);

	vector<double>::iterator min_y_result = min_element(yValues.begin(), yValues.end());
	vector<double>::iterator max_y_result = max_element(yValues.begin(), yValues.end());

	int min_y_index = distance(yValues.begin(), min_y_result);
	int max_y_index = distance(yValues.begin(), max_y_result);

	//basically finds a bounding box

	int xCurrent = (int)xValues[min_x_index];
	int xEnding = (int)xValues[max_x_index];
	int yCurrent = (int)yValues[min_y_index];
	int yEnding = (int)yValues[max_y_index];

	//cout << "First Point coordinates" <<firstPoint<<endl;
	//cout << "Second Point coordinates" <<secondPoint<<endl;
	//cout << "Third Point coordinates" <<thirdPoint<<endl;


	for(int i = xCurrent; i < xEnding ; i++){
		for(int j = yCurrent; j < yEnding ; j++){
			//cout<<"Current point is "<<i<<" "<<j<<endl;
			Vec4 currentPoint = Vec4(i,j,-1,-1,-1);
			double alpha = barycentricLineHelper(currentPoint,secondPoint,thirdPoint)/barycentricLineHelper(firstPoint,secondPoint,thirdPoint);
			double beta = barycentricLineHelper(currentPoint,thirdPoint,firstPoint)/barycentricLineHelper(secondPoint,thirdPoint,firstPoint);
			double gamma = barycentricLineHelper(currentPoint,firstPoint,secondPoint)/barycentricLineHelper(thirdPoint,firstPoint,secondPoint);
			//cout << "Alpha is " <<alpha<<endl;
			//cout << "Beta is " <<beta<<endl;
			//cout << "Gamma is " <<gamma<<endl;
			if(alpha >= 0 && beta >= 0 && gamma >= 0){
				//cout<<"Inside the if"<<endl;
				Color alphaColor = Color(0.0,0.0,0.0);
				Color betaColor = Color(0.0,0.0,0.0);
				Color gammaColor = Color(0.0,0.0,0.0);

				copyColorValues(&alphaColor,colorsOfVertices[firstPoint.colorId - 1]);
				copyColorValues(&betaColor,colorsOfVertices[secondPoint.colorId - 1]);
				copyColorValues(&gammaColor,colorsOfVertices[thirdPoint.colorId - 1]);

				multiplyColorWithScalar(&alphaColor , alpha);
				multiplyColorWithScalar(&betaColor ,beta);
				multiplyColorWithScalar(&gammaColor ,gamma);
				
				addTwoColors(&alphaColor,&alphaColor,&betaColor);
				addTwoColors(&alphaColor,&alphaColor,&gammaColor);
				markPixelwithColor(camera,i,j,alphaColor);
			}
		}
	}
}
double Scene::barycentricLineHelper(Vec4 & wantedPoint, Vec4 & firstPoint, Vec4 & secondPoint){
	return wantedPoint.x * (firstPoint.y - secondPoint.y) + wantedPoint.y * (secondPoint.x - firstPoint.x)+(firstPoint.x*secondPoint.y)-(firstPoint.y*secondPoint.x); 
}
/*

*/
pair<Vec4,Vec4> Scene::liangBarskyImplementation(Vec3 & minVertex , Vec3 & maxVertex, vector<Vec4> & copyVertices, int firstVertexIndex , int secondVertexIndex){ //pMin , pMax , x0, x1
	Vec4 firstVertex = copyVertices[firstVertexIndex];
	Vec4 secondVertex = copyVertices[secondVertexIndex];
	double tE = 0;
	double tL = 1;
	double dx = secondVertex.x - firstVertex.x;
	double dy = secondVertex.y - firstVertex.y;
	double dz = secondVertex.z - firstVertex.z;

	bool visible = false;
	if( isVisible(dx , minVertex.x - firstVertex.x  , tE , tL)){ //left
		if( isVisible(-dx , firstVertex.x - maxVertex.x , tE , tL)){ //right
			if( isVisible(dy , minVertex.y - firstVertex.y , tE , tL)){ //bottom
				if( isVisible(-dy , firstVertex.y - maxVertex.y , tE , tL)){ //top
					if( isVisible(dz , minVertex.z - firstVertex.z  , tE , tL)){ //front
						if( isVisible(-dz , firstVertex.z - maxVertex.z , tE , tL)){ //back
							//cout<<"Clipping exists"<<endl;
							visible = true;
							if(tL < 1){
								secondVertex.x = firstVertex.x + dx * tL;
								secondVertex.y = firstVertex.y + dy * tL;
								secondVertex.z = firstVertex.z + dz * tL;
							}
							if(tE > 0){
								firstVertex.x = firstVertex.x + dx * tE;
								firstVertex.y = firstVertex.y + dy * tE;
								firstVertex.z = firstVertex.z + dz * tE;
							}
						}
					}
				}
			}
		}
	}
	return make_pair(firstVertex,secondVertex);
}

bool Scene::isVisible(double den, double num , double & tE, double & tL){
	double t;
	if(den > 0.0){//potentially entering
		t = num / den;
		if(t > tL){
			return false;
		}
		if(t > tE){
			tE = t;
		}
	}
	else if(den < 0.0){ //potentially leaving
		t = num / den;
		if(t < tE){
			return false;
		}
		if(t < tL){
			tL = t;
		}
	}
	else{
		if(num > 0.0){
			return false;
		}
	}
	return true;
}

Matrix4 Scene::viewPortTransform(Camera * camera){
	//cout<<"ViewportTransformation"<<endl;
	double viewportTransformArray [4][4] = {
		{((double)camera->horRes)/2,0.0,0.0,((double)(camera->horRes-1))/2},
		{0.0,((double)camera->verRes)/2,0.0,((double)(camera->verRes-1))/2},
		{0.0,0.0,0.5,0.5},
		{0.0,0.0,0.0,0.0} //to make t argument equal to 0
	};
	Matrix4 viewportTransformMatrix = Matrix4(viewportTransformArray);
	return viewportTransformMatrix;
}

void Scene::doPairViewportTransformation(Camera * camera , pair<Vec4,Vec4> & clippedPair){
	Matrix4 transformMatrix = viewPortTransform(camera);
	clippedPair.first = multiplyMatrixWithVec4(transformMatrix,clippedPair.first);
	clippedPair.second = multiplyMatrixWithVec4(transformMatrix,clippedPair.second);

	roundVec4ToInteger(clippedPair.first);
	roundVec4ToInteger(clippedPair.second);
}

tuple<Vec4,Vec4,Vec4> Scene::doTriangleViewportTransformation(Camera * camera, vector<Vec4> & copyVertices , int firstPointIndex,int secondPointIndex,int thirdPointIndex ){
	Vec4 firstVertex = copyVertices[firstPointIndex];
	Vec4 secondVertex = copyVertices[secondPointIndex];
	Vec4 thirdVertex = copyVertices[thirdPointIndex];
	Matrix4 transformMatrix = viewPortTransform(camera);
	firstVertex = multiplyMatrixWithVec4(transformMatrix,firstVertex);
	secondVertex = multiplyMatrixWithVec4(transformMatrix,secondVertex);
	thirdVertex = multiplyMatrixWithVec4(transformMatrix,thirdVertex);
	roundVec4ToInteger(firstVertex);
	roundVec4ToInteger(secondVertex);
	roundVec4ToInteger(thirdVertex);

	tuple<Vec4,Vec4,Vec4> triangleTuple = make_tuple(
		firstVertex,
		secondVertex,
		thirdVertex
	);
	return triangleTuple;
}

void Scene::doOrthographicProjection(Camera * camera , vector<Vec4> & copyVertices){
	double orthographicTransformArray [4][4] = {
		{2/(camera->right - camera->left),0.0,0.0,-(camera->right + camera->left)/(camera->right - camera->left)},
		{0.0,2/(camera->top - camera->bottom),0.0,-(camera->top + camera->bottom)/(camera->top - camera->bottom)},
		{0.0,0.0,-2/(camera->far - camera->near),-(camera->far + camera->near)/(camera->far - camera->near)},
		{0.0,0.0,0.0,1.0}
	};
	Matrix4 orthographicTransformMatrix = Matrix4(orthographicTransformArray);
	//cout<<"Orthographic projection matrix : "<<orthographicTransformMatrix<<endl;
	for(int i = 0 ; i< copyVertices.size();i++){
		Vec4 currentVertexValues = copyVertices[i];
		//cout<<"Before ortographic projection : "<<currentVertexValues<<endl;
		Vec4 transformedVertex = multiplyMatrixWithVec4(orthographicTransformMatrix,currentVertexValues);
		//cout<<"After ortographic projection : "<<transformedVertex<<endl;
		copyVertices[i].x = transformedVertex.x;
		copyVertices[i].y = transformedVertex.y;
		copyVertices[i].z = transformedVertex.z;
		copyVertices[i].t = transformedVertex.t;
	}
}

void Scene::doPerspectiveProjection(Camera * camera , vector<Vec4> & copyVertices){
	double perspectiveTransformArray [4][4] = {
		{2*camera->near/(camera->right - camera->left),0.0,(camera->right + camera->left)/(camera->right - camera->left),0.0},
		{0.0,2*camera->near/(camera->top - camera->bottom),(camera->top + camera->bottom)/(camera->top - camera->bottom),0.0},
		{0.0,0.0,-(camera->far + camera->near)/(camera->far - camera->near),-(2*camera->far * camera->near)/(camera->far - camera->near)},
		{0.0,0.0,-1.0,0.0}
	};
	Matrix4 perspectiveTransformMatrix = Matrix4(perspectiveTransformArray);
	//cout<<"Perspective projection matrix : "<<perspectiveTransformMatrix<<endl;
	for(int i = 0 ; i< copyVertices.size();i++){
		Vec4 currentVertexValues = copyVertices[i];
		//cout<<"Before perspective projection : "<<currentVertexValues<<endl;
		Vec4 transformedVertex = multiplyMatrixWithVec4(perspectiveTransformMatrix,currentVertexValues);
		//cout<<"Before perspective projection : "<<currentVertexValues<<endl;
		copyVertices[i].x = transformedVertex.x;
		copyVertices[i].y = transformedVertex.y;
		copyVertices[i].z = transformedVertex.z;
		copyVertices[i].t = transformedVertex.t;
	}
}

void Scene::doViewingTransformation(Camera * camera , vector<Vec4> & copyVertices){
	//cout<<"Calls viewing transforms"<<endl;
	double viewTransformArray [4][4] = {
		{camera->u.x,camera->u.y,camera->u.z,-(camera->u.x*camera->pos.x + camera->u.y*camera->pos.y + camera->u.z*camera->pos.z)},
		{camera->v.x,camera->v.y,camera->v.z,-(camera->v.x*camera->pos.x + camera->v.y*camera->pos.y + camera->v.z*camera->pos.z)},
		{camera->w.x,camera->w.y,camera->w.z,-(camera->w.x*camera->pos.x + camera->w.y*camera->pos.y + camera->w.z*camera->pos.z)},
		{0.0,0.0,0.0,1.0}
	};
	Matrix4 viewTransformMatrix = Matrix4(viewTransformArray);
	//cout<<"Current viewing transformation : "<<viewTransformMatrix<<endl;
	for(int i = 0 ; i < copyVertices.size() ; i++){
		Vec4 currentVertexValues = copyVertices[i];
		//cout<<"Before viewing transformation : "<<currentVertexValues<<endl;
		Vec4 transformedVertex = multiplyMatrixWithVec4(viewTransformMatrix,currentVertexValues);
		//cout<<"After viewing transformation : "<<transformedVertex<<endl;
		copyVertices[i].x = transformedVertex.x;
		copyVertices[i].y = transformedVertex.y;
		copyVertices[i].z = transformedVertex.z;
		copyVertices[i].t = transformedVertex.t;
	}
}

Matrix4 Scene::translationModellingTransform(Translation * translation){
    double transformArray[4][4] = {{1.0,0.0,0.0,translation->tx},
                            {0.0,1.0,0.0,translation->ty},
                            {0.0,0.0,1.0,translation->tz},
                            {0.0,0.0,0.0,1.0}
                            };
    Matrix4 transformingMatrix = Matrix4(transformArray);
    return transformingMatrix;
}

Matrix4 Scene::scalingModellingTransform(Scaling * scaling){
    double transformArray[4][4] = {{scaling->sx,0,0,0},
                                {0,scaling->sy,0,0},
                                {0,0,scaling->sz,0},
                                {0,0,0,1}
                                };
    Matrix4 transformingMatrix = Matrix4(transformArray);
    return transformingMatrix;
}

Matrix4 Scene::rotatingModellingTransform(Rotation * rotation){
	vector<double> rotationAxisChooser;
	rotationAxisChooser.push_back(abs(rotation->ux));
	rotationAxisChooser.push_back(abs(rotation->uy));
	rotationAxisChooser.push_back(abs(rotation->uz));
	vector<double>::iterator min_result = min_element(rotationAxisChooser.begin(), rotationAxisChooser.end());
	int min_index = distance(rotationAxisChooser.begin(), min_result);
	//std::cout << "min_result at: " << min_index;
    Vec3 uVector = Vec3(rotation->ux , rotation->uy, rotation->uz,-1);
    //make it to unitVector
    uVector = normalizeVec3(uVector);
    Vec3 vVector;
    Vec3 wVector;
	switch(min_index){
		case 0:{
			//x is min
            vVector = Vec3();
            vVector.y = uVector.z;
            vVector.z = -uVector.y;
			break;
		}
		case 1:{
			//y is min
            vVector = Vec3();
            vVector.x = uVector.z;
            vVector.z = -uVector.x;
			break;
		}
		case 2:{
			//z is min
            vVector = Vec3();
            vVector.x = uVector.y;
            vVector.y = -uVector.x;
			break;
		}
		default : {
			cout<<"rotate index out of bounds"<<endl;
			exit(-1);
			break;
		}
	}
    wVector = crossProductVec3(uVector,vVector);
	vVector = normalizeVec3(vVector);
    wVector = normalizeVec3(wVector);
    double transformArray [4][4] = {{uVector.x,uVector.y,uVector.z,0.0},
                                    {vVector.x,vVector.y,vVector.z,0.0},
                                    {wVector.x,wVector.y,wVector.z,0.0},
                                    {0.0,0.0,0.0,1.0}
                                    };
    double rotationAngleInRadians = (rotation->angle / 180.0) * M_PI;
    double rotationXArray [4][4] = {{1.0,0.0,0.0,0.0},
                                    {0.0,cos(rotationAngleInRadians),-sin(rotationAngleInRadians),0.0},
                                    {0.0,sin(rotationAngleInRadians),cos(rotationAngleInRadians),0.0},
                                    {0.0,0.0,0.0,1.0}
                                    };
    double inverseTransformArray [4][4] = {{uVector.x,vVector.x,wVector.x,0.0},
                                    {uVector.y,vVector.y,wVector.y,0.0},
                                    {uVector.z,vVector.z,wVector.z,0.0},
                                    {0.0,0.0,0.0,1.0}
                                    };
    Matrix4 transformingMatrix = Matrix4(transformArray);
    Matrix4 xRotationMatrix = Matrix4(rotationXArray);
    Matrix4 inverseTransformingMatrix = Matrix4(inverseTransformArray);
    transformingMatrix = multiplyMatrixWithMatrix(xRotationMatrix,transformingMatrix);
    transformingMatrix = multiplyMatrixWithMatrix(inverseTransformingMatrix,transformingMatrix);
    return transformingMatrix;
}


void Scene::doModellingTransformation(char mTransformType, int mTransformId, set <int, greater <int> > & vertexSet , vector<Vec4> & copyVertices){
	//here vertexSet is indexed from 1, act according to it!
	set <int, greater <int> > :: iterator v_itr;
	Matrix4 transformMatrix;
	switch(mTransformType){
		case 'r':{
			Rotation * currentRotation = rotations[mTransformId];
			transformMatrix = rotatingModellingTransform(currentRotation);
			//cout<<"Modelling transformation r is "<<*currentRotation<<endl;
			break;
		}
		case 's':{
			Scaling * currentScaling = scalings[mTransformId];
			transformMatrix = scalingModellingTransform(currentScaling);
			//cout<<"Modelling transformation s is "<<*currentScaling<<endl;
			break;
		}
		case 't':{
			Translation * currentTranslation = translations[mTransformId];
			transformMatrix = translationModellingTransform(currentTranslation);
			//cout<<"Modelling transformation t is "<<*currentTranslation<<endl;
			break;
		}
		default:{
			//cout<<"Invalid modelling transformation"<<endl;
			exit(-1);
			break;
		}
	}
	for (v_itr = vertexSet.begin(); v_itr != vertexSet.end(); ++v_itr){
		int currentVertexId = (*v_itr) -1 ;
		Vec4 currentVertexValues = copyVertices[currentVertexId];
		//cout<<"Before modelling transformation :"<<currentVertexValues<<endl;

		Vec4 transformedVertex = multiplyMatrixWithVec4(transformMatrix,currentVertexValues);
		//cout<<"After modelling transformation :"<<transformedVertex<<endl;
		copyVertices[currentVertexId].x = transformedVertex.x;
		copyVertices[currentVertexId].y = transformedVertex.y;
		copyVertices[currentVertexId].z = transformedVertex.z;
		copyVertices[currentVertexId].t = transformedVertex.t;
	}
}
/*
	Parses XML file
*/
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLElement *pElement;

	xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	// read background color
	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%lf %lf %lf", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	// read culling
	pElement = pRoot->FirstChildElement("Culling");
	if (pElement != NULL)
		pElement->QueryBoolText(&cullingEnabled);

	// read projection type
	pElement = pRoot->FirstChildElement("ProjectionType");
	if (pElement != NULL)
		pElement->QueryIntText(&projectionType);

	// read cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while (pCamera != NULL)
	{
		Camera *cam = new Camera();

		pCamera->QueryIntAttribute("id", &cam->cameraId);

		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->pos.x, &cam->pos.y, &cam->pos.z);

		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->gaze.x, &cam->gaze.y, &cam->gaze.z);

		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf", &cam->v.x, &cam->v.y, &cam->v.z);

		cam->gaze = normalizeVec3(cam->gaze);
		cam->u = crossProductVec3(cam->gaze, cam->v);
		cam->u = normalizeVec3(cam->u);

		cam->w = inverseVec3(cam->gaze);
		cam->v = crossProductVec3(cam->u, cam->gaze);
		cam->v = normalizeVec3(cam->v);

		camElement = pCamera->FirstChildElement("ImagePlane");
		str = camElement->GetText();
		sscanf(str, "%lf %lf %lf %lf %lf %lf %d %d",
			   &cam->left, &cam->right, &cam->bottom, &cam->top,
			   &cam->near, &cam->far, &cam->horRes, &cam->verRes);

		camElement = pCamera->FirstChildElement("OutputName");
		str = camElement->GetText();
		cam->outputFileName = string(str);

		cameras.push_back(cam);

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// read vertices
	pElement = pRoot->FirstChildElement("Vertices");
	XMLElement *pVertex = pElement->FirstChildElement("Vertex");
	int vertexId = 1;

	while (pVertex != NULL)
	{
		Vec3 *vertex = new Vec3();
		Color *color = new Color();

		vertex->colorId = vertexId;

		str = pVertex->Attribute("position");
		sscanf(str, "%lf %lf %lf", &vertex->x, &vertex->y, &vertex->z);

		str = pVertex->Attribute("color");
		sscanf(str, "%lf %lf %lf", &color->r, &color->g, &color->b);

		vertices.push_back(vertex);
		colorsOfVertices.push_back(color);

		pVertex = pVertex->NextSiblingElement("Vertex");

		vertexId++;
	}

	// read translations
	pElement = pRoot->FirstChildElement("Translations");
	XMLElement *pTranslation = pElement->FirstChildElement("Translation");
	while (pTranslation != NULL)
	{
		Translation *translation = new Translation();

		pTranslation->QueryIntAttribute("id", &translation->translationId);

		str = pTranslation->Attribute("value");
		sscanf(str, "%lf %lf %lf", &translation->tx, &translation->ty, &translation->tz);

		translations.push_back(translation);

		pTranslation = pTranslation->NextSiblingElement("Translation");
	}

	// read scalings
	pElement = pRoot->FirstChildElement("Scalings");
	XMLElement *pScaling = pElement->FirstChildElement("Scaling");
	while (pScaling != NULL)
	{
		Scaling *scaling = new Scaling();

		pScaling->QueryIntAttribute("id", &scaling->scalingId);
		str = pScaling->Attribute("value");
		sscanf(str, "%lf %lf %lf", &scaling->sx, &scaling->sy, &scaling->sz);

		scalings.push_back(scaling);

		pScaling = pScaling->NextSiblingElement("Scaling");
	}

	// read rotations
	pElement = pRoot->FirstChildElement("Rotations");
	XMLElement *pRotation = pElement->FirstChildElement("Rotation");
	while (pRotation != NULL)
	{
		Rotation *rotation = new Rotation();

		pRotation->QueryIntAttribute("id", &rotation->rotationId);
		str = pRotation->Attribute("value");
		sscanf(str, "%lf %lf %lf %lf", &rotation->angle, &rotation->ux, &rotation->uy, &rotation->uz);

		rotations.push_back(rotation);

		pRotation = pRotation->NextSiblingElement("Rotation");
	}

	// read models
	pElement = pRoot->FirstChildElement("Models");

	XMLElement *pModel = pElement->FirstChildElement("Model");
	XMLElement *modelElement;
	while (pModel != NULL)
	{
		Model *model = new Model();

		pModel->QueryIntAttribute("id", &model->modelId);
		pModel->QueryIntAttribute("type", &model->type);

		// read model transformations
		XMLElement *pTransformations = pModel->FirstChildElement("Transformations");
		XMLElement *pTransformation = pTransformations->FirstChildElement("Transformation");

		pTransformations->QueryIntAttribute("count", &model->numberOfTransformations);

		while (pTransformation != NULL)
		{
			char transformationType;
			int transformationId;

			str = pTransformation->GetText();
			sscanf(str, "%c %d", &transformationType, &transformationId);

			model->transformationTypes.push_back(transformationType);
			model->transformationIds.push_back(transformationId);

			pTransformation = pTransformation->NextSiblingElement("Transformation");
		}

		// read model triangles
		XMLElement *pTriangles = pModel->FirstChildElement("Triangles");
		XMLElement *pTriangle = pTriangles->FirstChildElement("Triangle");

		pTriangles->QueryIntAttribute("count", &model->numberOfTriangles);

		while (pTriangle != NULL)
		{
			int v1, v2, v3;

			str = pTriangle->GetText();
			sscanf(str, "%d %d %d", &v1, &v2, &v3);

			model->triangles.push_back(Triangle(v1, v2, v3));

			pTriangle = pTriangle->NextSiblingElement("Triangle");
		}

		models.push_back(model);

		pModel = pModel->NextSiblingElement("Model");
	}
}

/*
	Initializes image with background color
*/
void Scene::initializeImage(Camera *camera)
{
	if (this->image.empty())
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			vector<Color> rowOfColors;

			for (int j = 0; j < camera->verRes; j++)
			{
				rowOfColors.push_back(this->backgroundColor);
			}

			this->image.push_back(rowOfColors);
		}
	}
	// if image is filled before, just change color rgb values with the background color
	else
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			for (int j = 0; j < camera->verRes; j++)
			{
				this->image[i][j].r = this->backgroundColor.r;
				this->image[i][j].g = this->backgroundColor.g;
				this->image[i][j].b = this->backgroundColor.b;
			}
		}
	}
}

/*
	If given value is less than 0, converts value to 0.
	If given value is more than 255, converts value to 255.
	Otherwise returns value itself.
*/
int Scene::makeBetweenZeroAnd255(double value)
{
	if (value >= 255.0)
		return 255;
	if (value <= 0.0)
		return 0;
	return (int)(value);
}

/*
	Writes contents of image (Color**) into a PPM file.
*/
void Scene::writeImageToPPMFile(Camera *camera)
{
	ofstream fout;

	fout.open(camera->outputFileName.c_str());

	fout << "P3" << endl;
	fout << "# " << camera->outputFileName << endl;
	fout << camera->horRes << " " << camera->verRes << endl;
	fout << "255" << endl;

	for (int j = camera->verRes - 1; j >= 0; j--)
	{
		for (int i = 0; i < camera->horRes; i++)
		{
			fout << makeBetweenZeroAnd255(this->image[i][j].r) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].g) << " "
				 << makeBetweenZeroAnd255(this->image[i][j].b) << " ";
		}
		fout << endl;
	}
	fout.close();
}

/*
	Converts PPM image in given path to PNG file, by calling ImageMagick's 'convert' command.
	os_type == 1 		-> Ubuntu
	os_type == 2 		-> Windows
	os_type == other	-> No conversion
*/
void Scene::convertPPMToPNG(string ppmFileName, int osType)
{
	string command;

	// call command on Ubuntu
	if (osType == 1)
	{
		command = "convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// call command on Windows
	else if (osType == 2)
	{
		command = "magick convert " + ppmFileName + " " + ppmFileName + ".png";
		system(command.c_str());
	}

	// default action - don't do conversion
	else
	{
	}
}