#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#include <set> 
#include <iterator>
#include <utility>
#include <algorithm>
#include <cmath>
#include <tuple>

#include "Camera.h"
#include "Color.h"
#include "Model.h"
#include "Rotation.h"
#include "Scaling.h"
#include "Translation.h"
#include "Triangle.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix4.h"


using namespace std;

class Scene
{
public:
	Color backgroundColor;
	bool cullingEnabled;
	int projectionType;

	vector< vector<Color> > image;
	vector< Camera* > cameras;
	vector< Vec3* > vertices;
	vector< Color* > colorsOfVertices;
	vector< Scaling* > scalings;
	vector< Rotation* > rotations;
	vector< Translation* > translations;
	vector< Model* > models;

	Scene(const char *xmlPath);

	void initializeImage(Camera* camera);
	Matrix4 translationModellingTransform(Translation * translation);
	Matrix4 rotatingModellingTransform(Rotation * rotation);
	Matrix4 scalingModellingTransform(Scaling * scaling);
	Matrix4 viewPortTransform(Camera * camera);
	void doModellingTransformation(char mTransformType, int mTransformId, set <int, greater <int> > & vertexSet, vector<Vec4> & copyVertices);
	void doViewingTransformation(Camera * camera , vector<Vec4> & copyVertices);
	void doOrthographicProjection(Camera * camera , vector<Vec4> & copyVertices);
	void doPerspectiveProjection(Camera * camera , vector<Vec4> & copyVertices);
	bool isVisible(double den, double num , double & tE, double & tL);
	pair<Vec4,Vec4> liangBarskyImplementation(Vec3 & minVertex , Vec3 & maxVertex, vector<Vec4> & copyVertices, int firstVertexIndex , int secondVertexIndex);
	void doPerspectiveDivide(vector<Vec4> & copyVertices);
	void doPairViewportTransformation(Camera * camera , pair<Vec4,Vec4> & clippedPair);
	tuple<Vec4,Vec4,Vec4> doTriangleViewportTransformation(Camera * camera, vector<Vec4> & copyVertices , int firstPointIndex,int secondPointIndex,int thirdPointIndex );
	bool checkBackfaceCulling(Camera * camera , vector<Vec4> & copyVertices, int firstPointIndex, int secondPointIndex , int thirdPointIndex);
	void lineRasterizer(Camera * camera ,pair<Vec4,Vec4> & clippedPair );
	void barycentricRasterizer(Camera * camera,tuple<Vec4,Vec4,Vec4> & triangleTuple);
	double barycentricLineHelper(Vec4 & wantedPoint, Vec4 & firstPoint, Vec4 & secondPoint);
	void markPixelwithColor(Camera * camera,int x , int y , Color & colorInfo);
	void forwardRenderingPipeline(Camera* camera);
	int makeBetweenZeroAnd255(double value);
	void writeImageToPPMFile(Camera* camera);
	void convertPPMToPNG(string ppmFileName, int osType);
};

#endif
