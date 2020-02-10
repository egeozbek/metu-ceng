#include "Scene.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Shape.h"

#include "tinyxml2.h"


using namespace tinyxml2;

/* 
 * Must render the scene from each camera's viewpoint and create an image.
 * You can use the methods of the Image class to save the image as a PPM file. 
 */

Color Scene :: getBackgroundColor(){
	unsigned char tempRed = (unsigned char) pScene->backgroundColor.r;
	unsigned char tempGreen = (unsigned char) pScene->backgroundColor.g;
	unsigned char tempBlue = (unsigned char) pScene->backgroundColor.b;

	Color backgroundColor = { tempRed,tempGreen,tempBlue};

	return backgroundColor;

}


float Scene::getDistanceBetweenTwoPoints(const Vector3f & first,const Vector3f & second) const{
	return sqrt(
		pow(first.x - second.x,2.0) +
		pow(first.y - second.y,2.0) +
		pow(first.z - second.z,2.0)
	);
}

Vector3f Scene::ambientLightningColor(int materialId) const{
	Vector3f ambientReflectance = pScene->materials[materialId-1]->ambientRef;
	Vector3f multiplicationResult = vectorUtils.vectorElementwiseMultiplication(ambientReflectance,pScene->ambientLight);
	return multiplicationResult;
}
Vector3f Scene::diffuseLightningColor(const Vector3f & diffuseReflectionCoefficients, PointLight * currentLightSource, const Vector3f & intersectionPoint , const Vector3f & directionToLightSourceNormalized , const Vector3f & unitNormal) const{

	Vector3f currentColorContributions = {
		0.0,
		0.0,
		0.0
	};

	//calculate the cosine between light source and unit normal
	float cosine = max((float)0.0,vectorUtils.dotProduct(unitNormal,directionToLightSourceNormalized));

	//calculate the irradience for that point
	Vector3f lightContribution = currentLightSource->computeLightContribution(intersectionPoint);

	Vector3f middleCoefficient = vectorUtils.vectorElementwiseMultiplication(diffuseReflectionCoefficients,lightContribution);
	Vector3f finalCoefficients = vectorUtils.vectorMultiplication(middleCoefficient,cosine);
	currentColorContributions = vectorUtils.vectorAddition(currentColorContributions,finalCoefficients);

	return currentColorContributions;
}

Vector3f Scene::specularLightningColor(const Vector3f & specularReflectionCoefficients, int phongExponent , PointLight * currentLightSource, const Vector3f & intersectionPoint,const Vector3f & vectorToCamera, const Vector3f &  directionToLightSourceNormalized, const Vector3f & unitNormal) const{
	Vector3f currentColorContributions = {
		0.0,
		0.0,
		0.0
	};

	Vector3f halfVector = vectorUtils.vectorAddition(vectorToCamera,directionToLightSourceNormalized);
	Vector3f halfVectorNormalized = vectorUtils.normalizeVector(halfVector);

	float cosine = max((float)0.0,vectorUtils.dotProduct(unitNormal,halfVectorNormalized));
	float cosinePowered = pow(cosine , phongExponent);
	//std::cout<<"Cosine powered "<<cosinePowered<<std::endl;

	Vector3f lightContribution = currentLightSource->computeLightContribution(intersectionPoint);

	Vector3f middleCoefficient = vectorUtils.vectorElementwiseMultiplication(specularReflectionCoefficients,lightContribution);
	Vector3f finalCoefficients = vectorUtils.vectorMultiplication(middleCoefficient,cosinePowered);
	currentColorContributions = vectorUtils.vectorAddition(currentColorContributions,finalCoefficients);
		
	return currentColorContributions;

}

Vector3f Scene::getLocalColor(int materialId ,const Vector3f & intersectionPoint ,  const Vector3f & unitNormal, const Vector3f & cameraOrigin) const{
	Vector3f totalColorVector = {
		0.0,
		0.0,
		0.0
	};

	int numberOfLights = pScene->lights.size();
	Vector3f diffuseReflectionCoefficients = pScene->materials[materialId-1]->diffuseRef;

	int phongExponent = pScene -> materials[materialId -1]->phongExp;
	Vector3f specularReflectionCoefficients = pScene->materials[materialId-1]->specularRef;

	for(int i = 0 ; i < numberOfLights ; i++){
		//check if it's in shadow for that light source
		PointLight * currentLightSource = pScene->lights[i];

		Vector3f directionToLightSource = vectorUtils.vectorSubtraction(currentLightSource->position,intersectionPoint);
		Vector3f directionToLightSourceNormalized = vectorUtils.normalizeVector(directionToLightSource);

		Vector3f newStartingPointDirection = vectorUtils.vectorMultiplication(directionToLightSourceNormalized,pScene->shadowRayEps);
		Vector3f newStartingPoint = vectorUtils.vectorAddition(intersectionPoint,newStartingPointDirection);
		Ray shadowRay (newStartingPoint,directionToLightSourceNormalized);

		ReturnVal shadowRayIntersectionResults = traceShadowRay(shadowRay);

		bool isInShadow = false;

		if(shadowRayIntersectionResults.isIntersected){
			//SHADOW INTERSECTS THE OBJECT , CHECK IF ITS BEFORE OR AFTER THE LIGHT
			float distanceToObjectIntersection = getDistanceBetweenTwoPoints(intersectionPoint,shadowRayIntersectionResults.intersectionPoint);
			float distanceToLightSource = getDistanceBetweenTwoPoints(intersectionPoint,currentLightSource->position);
			if(distanceToLightSource > distanceToObjectIntersection){
				isInShadow = true;
			}			
		}

		//ELSE -> SHADOWRAY DOESN'T INTERSECT OBJECT its not in the shadow 

		if(!isInShadow){
			Vector3f vectorToCamera = vectorUtils.vectorSubtraction(cameraOrigin,intersectionPoint);
			vectorToCamera = vectorUtils.normalizeVector(vectorToCamera);

			//diffuse

			Vector3f diffuseColor = diffuseLightningColor(diffuseReflectionCoefficients,
			currentLightSource,
			intersectionPoint,
			directionToLightSourceNormalized,
			unitNormal);

			Vector3f specularColor = specularLightningColor(specularReflectionCoefficients,
			phongExponent,
			currentLightSource,
			intersectionPoint,
			vectorToCamera ,
			directionToLightSourceNormalized,
			unitNormal);

			Vector3f totalColorAddition = vectorUtils.vectorAddition(diffuseColor,specularColor);
			totalColorVector = vectorUtils.vectorAddition(totalColorVector,totalColorAddition);
		}
	}

	return totalColorVector;
}

ReturnVal Scene :: traceRay(const Ray & ray , const int recursionLeft )const {
	

	//if recursionsLeft is 0 dont allow recursion to be called , just return current intersection

	int numberOfObjects = pScene -> objects.size();

	ReturnVal generalRayIntersectionResult =
	{
		false,
		-1,
		{0.0,0.0,0.0},
		{0.0,0.0,0.0},
		{0.0,0.0,0.0}
	};

	float minIntersectionDistance = std::numeric_limits<float>::max();

	for(int p = 0 ; p < numberOfObjects ; p++){
		ReturnVal intersectionResult = pScene->objects[p]->intersect(ray);

		if(intersectionResult.isIntersected){
			generalRayIntersectionResult.isIntersected = true;
			float newDistance = getDistanceBetweenTwoPoints(ray.origin,intersectionResult.intersectionPoint); 
			if( newDistance < minIntersectionDistance){

				minIntersectionDistance = newDistance;
				generalRayIntersectionResult.materialId = pScene->objects[p]->matIndex;
				generalRayIntersectionResult.intersectionPoint = intersectionResult.intersectionPoint;
				generalRayIntersectionResult.unitSurfaceNormal = intersectionResult.unitSurfaceNormal;

			}
		}
	}

	if(!generalRayIntersectionResult.isIntersected){
		return generalRayIntersectionResult;
	}
	else{

		//there is an intersection calculate local color on intersection point
		Vector3f diffAndSpecAtPoint = getLocalColor(generalRayIntersectionResult.materialId,
			generalRayIntersectionResult.intersectionPoint,
			generalRayIntersectionResult.unitSurfaceNormal,
			ray.origin);
	
		generalRayIntersectionResult.reflectionContributions = diffAndSpecAtPoint;
		

		Vector3f reflectionCoefficients = pScene->materials[generalRayIntersectionResult.materialId -1 ]->mirrorRef;

		bool areReflectionCoefficientsZero = vectorUtils.isVectorAllZeros(reflectionCoefficients);
		if( !areReflectionCoefficientsZero && recursionLeft > 0){
			std::cout<<"Allowed reflection detected"<<std::endl;

			//object is reflective , calculate the reflection ray
			Vector3f normalizedViewRay = vectorUtils.normalizeVector(ray.direction);

			float angleBetween = vectorUtils.dotProduct(normalizedViewRay,generalRayIntersectionResult.unitSurfaceNormal);
			float mutlipliedWithTwo = -2.0 * angleBetween;

			Vector3f beforeAddingViewRay = vectorUtils.vectorMultiplication(generalRayIntersectionResult.unitSurfaceNormal,mutlipliedWithTwo);
            Vector3f reflectionRayDirection = vectorUtils.vectorAddition(beforeAddingViewRay,normalizedViewRay);

            Vector3f normalizedReflectionRayDirection = vectorUtils.normalizeVector(reflectionRayDirection);

			//shift starting point , so no self collision
			Vector3f newStartingShift = vectorUtils.vectorMultiplication(normalizedReflectionRayDirection,pScene->shadowRayEps);
			Vector3f reflectionStartingPoint = vectorUtils.vectorAddition(generalRayIntersectionResult.intersectionPoint,newStartingShift);

			std::cout<<"Reflection ray calculated"<<std::endl;

			//Create a reflection ray
			Ray reflectionRay (reflectionStartingPoint , normalizedReflectionRayDirection);
			std::cout<<"Reflection ray created"<<std::endl;

			//Get light contributions from recursion
			ReturnVal reflectionResults = traceRay(reflectionRay, recursionLeft-1);

			std::cout<<"Reflection recursion calculated"<<std::endl;

			// multiply reflection results with reflection coefficients to calculate new values
			Vector3f modifiedReflectionColors = vectorUtils.vectorElementwiseMultiplication(reflectionResults.reflectionContributions,reflectionCoefficients);
			std::cout<<"Modified colors calculated"<<std::endl;

			// add to the existing local color			
			generalRayIntersectionResult.reflectionContributions = vectorUtils.vectorAddition(generalRayIntersectionResult.reflectionContributions,modifiedReflectionColors);
			std::cout<<"Modified colors added"<<std::endl;
			
			return generalRayIntersectionResult;
		}
		else{
			//object is not reflective , just return the results
			return generalRayIntersectionResult;
		}
	}
}

ReturnVal Scene :: traceShadowRay(const Ray & ray)const {

    int numberOfObjects = pScene -> objects.size();

    ReturnVal generalRayIntersectionResult =
            {
                    false,
                    -1,
                    {0.0,0.0,0.0},
                    {0.0,0.0,0.0},
                    {0.0,0.0,0.0}
            };

    float minIntersectionDistance = std::numeric_limits<float>::max();

    for(int p = 0 ; p < numberOfObjects ; p++){
        ReturnVal intersectionResult = pScene->objects[p]->intersect(ray);

        if(intersectionResult.isIntersected){
            generalRayIntersectionResult.isIntersected = true;
            float newDistance = getDistanceBetweenTwoPoints(ray.origin,intersectionResult.intersectionPoint);
            if( newDistance < minIntersectionDistance){

                minIntersectionDistance = newDistance;
                generalRayIntersectionResult.materialId = pScene->objects[p]->matIndex;
                generalRayIntersectionResult.intersectionPoint = intersectionResult.intersectionPoint;
                generalRayIntersectionResult.unitSurfaceNormal = intersectionResult.unitSurfaceNormal;

            }
        }
    }

    return generalRayIntersectionResult;
}



void Scene::renderScene(void)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
	int numberOfCameras = pScene ->cameras.size();
	for(int cameraIndex = 0; cameraIndex < numberOfCameras ; cameraIndex++){
		Camera * currentCamera = pScene->cameras[cameraIndex];
		int imageWidth = currentCamera->imgPlane.nx;
		int imageHeight = currentCamera->imgPlane.ny;
		int reflectionRecursionLimit = pScene->maxRecursionDepth;

		Image currentImage (imageWidth,imageHeight);

		for(int row = 0 ; row < imageWidth ; row++){
			for(int col = 0 ; col < imageHeight ; col++ ){
				Ray currentRay = currentCamera->getPrimaryRay(row,col);

				ReturnVal currentTrace = traceRay(currentRay,reflectionRecursionLimit);

				//just send a single ray, see if it intersects with anything

				if(!currentTrace.isIntersected){
					//ray doesn't intersect anything just return background color
					Color backgroundColor = getBackgroundColor();
					currentImage.setPixelValue(col,row,backgroundColor);
				}
				else{
					//it intersected something, get shading for that point , recursive call if mirror( with maxDepth -1 trace)
					Vector3f ambientColor = ambientLightningColor(currentTrace.materialId);

					Vector3f totalColorVector = vectorUtils.vectorAddition(ambientColor,currentTrace.reflectionContributions);
					Color intersectionColor = vectorUtils.vecToColorWithClamp(totalColorVector);
					currentImage.setPixelValue(col,row,intersectionColor);
				}
			}
		}
		std::cout<<"Saving Image"<<std::endl;
		currentImage.saveImage(pScene->cameras[cameraIndex]->imageName);
	}
}

// Parses XML file. 
Scene::Scene(const char *xmlPath)
{
	const char *str;
	XMLDocument xmlDoc;
	XMLError eResult;
	XMLElement *pElement;

	maxRecursionDepth = 1;
	shadowRayEps = 0.001;

	eResult = xmlDoc.LoadFile(xmlPath);

	XMLNode *pRoot = xmlDoc.FirstChild();

	pElement = pRoot->FirstChildElement("MaxRecursionDepth");
	if(pElement != nullptr)
		pElement->QueryIntText(&maxRecursionDepth);

	pElement = pRoot->FirstChildElement("BackgroundColor");
	str = pElement->GetText();
	sscanf(str, "%f %f %f", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);

	pElement = pRoot->FirstChildElement("ShadowRayEpsilon");
	if(pElement != nullptr)
		pElement->QueryFloatText(&shadowRayEps);

	pElement = pRoot->FirstChildElement("IntersectionTestEpsilon");
	if(pElement != nullptr)
		eResult = pElement->QueryFloatText(&intTestEps);

	// Parse cameras
	pElement = pRoot->FirstChildElement("Cameras");
	XMLElement *pCamera = pElement->FirstChildElement("Camera");
	XMLElement *camElement;
	while(pCamera != nullptr)
	{
        int id;
        char imageName[64];
        Vector3f pos, gaze, up;
        ImagePlane imgPlane;

		eResult = pCamera->QueryIntAttribute("id", &id);
		camElement = pCamera->FirstChildElement("Position");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &pos.x, &pos.y, &pos.z);
		camElement = pCamera->FirstChildElement("Gaze");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &gaze.x, &gaze.y, &gaze.z);
		camElement = pCamera->FirstChildElement("Up");
		str = camElement->GetText();
		sscanf(str, "%f %f %f", &up.x, &up.y, &up.z);
		camElement = pCamera->FirstChildElement("NearPlane");
		str = camElement->GetText();
		sscanf(str, "%f %f %f %f", &imgPlane.left, &imgPlane.right, &imgPlane.bottom, &imgPlane.top);
		camElement = pCamera->FirstChildElement("NearDistance");
		eResult = camElement->QueryFloatText(&imgPlane.distance);
		camElement = pCamera->FirstChildElement("ImageResolution");	
		str = camElement->GetText();
		sscanf(str, "%d %d", &imgPlane.nx, &imgPlane.ny);
		camElement = pCamera->FirstChildElement("ImageName");
		str = camElement->GetText();
		strcpy(imageName, str);

		cameras.push_back(new Camera(id, imageName, pos, gaze, up, imgPlane));

		pCamera = pCamera->NextSiblingElement("Camera");
	}

	// Parse materals
	pElement = pRoot->FirstChildElement("Materials");
	XMLElement *pMaterial = pElement->FirstChildElement("Material");
	XMLElement *materialElement;
	while(pMaterial != nullptr)
	{
		materials.push_back(new Material());

		int curr = materials.size() - 1;
	
		eResult = pMaterial->QueryIntAttribute("id", &materials[curr]->id);
		materialElement = pMaterial->FirstChildElement("AmbientReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->ambientRef.r, &materials[curr]->ambientRef.g, &materials[curr]->ambientRef.b);
		materialElement = pMaterial->FirstChildElement("DiffuseReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->diffuseRef.r, &materials[curr]->diffuseRef.g, &materials[curr]->diffuseRef.b);
		materialElement = pMaterial->FirstChildElement("SpecularReflectance");
		str = materialElement->GetText();
		sscanf(str, "%f %f %f", &materials[curr]->specularRef.r, &materials[curr]->specularRef.g, &materials[curr]->specularRef.b);
		materialElement = pMaterial->FirstChildElement("MirrorReflectance");
		if(materialElement != nullptr)
		{
			str = materialElement->GetText();
			sscanf(str, "%f %f %f", &materials[curr]->mirrorRef.r, &materials[curr]->mirrorRef.g, &materials[curr]->mirrorRef.b);
		}
				else
		{
			materials[curr]->mirrorRef.r = 0.0;
			materials[curr]->mirrorRef.g = 0.0;
			materials[curr]->mirrorRef.b = 0.0;
		}
		materialElement = pMaterial->FirstChildElement("PhongExponent");
		if(materialElement != nullptr)
			materialElement->QueryIntText(&materials[curr]->phongExp);

		pMaterial = pMaterial->NextSiblingElement("Material");
	}

	// Parse vertex data
	pElement = pRoot->FirstChildElement("VertexData");
	int cursor = 0;
	Vector3f tmpPoint;
	str = pElement->GetText();
	while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
		cursor++;
	while(str[cursor] != '\0')
	{
		for(int cnt = 0 ; cnt < 3 ; cnt++)
		{
			if(cnt == 0)
				tmpPoint.x = atof(str + cursor);
			else if(cnt == 1)
				tmpPoint.y = atof(str + cursor);
			else
				tmpPoint.z = atof(str + cursor);
			while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
				cursor++; 
			while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
				cursor++;
		}
		vertices.push_back(tmpPoint);
	}

	// Parse objects
	pElement = pRoot->FirstChildElement("Objects");
	
	// Parse spheres
	XMLElement *pObject = pElement->FirstChildElement("Sphere");
	XMLElement *objElement;
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int cIndex;
		float R;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Center");
		eResult = objElement->QueryIntText(&cIndex);
		objElement = pObject->FirstChildElement("Radius");
		eResult = objElement->QueryFloatText(&R);

		objects.push_back(new Sphere(id, matIndex, cIndex, R));

		pObject = pObject->NextSiblingElement("Sphere");
	}

	// Parse triangles
	pObject = pElement->FirstChildElement("Triangle");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Indices");
		str = objElement->GetText();
		sscanf(str, "%d %d %d", &p1Index, &p2Index, &p3Index);

		objects.push_back(new Triangle(id, matIndex, p1Index, p2Index, p3Index));

		pObject = pObject->NextSiblingElement("Triangle");
	}

	// Parse meshes
	pObject = pElement->FirstChildElement("Mesh");
	while(pObject != nullptr)
	{
		int id;
		int matIndex;
		int p1Index;
		int p2Index;
		int p3Index;
		int cursor = 0;
		int vertexOffset = 0;
		vector<Triangle> faces;

		eResult = pObject->QueryIntAttribute("id", &id);
		objElement = pObject->FirstChildElement("Material");
		eResult = objElement->QueryIntText(&matIndex);
		objElement = pObject->FirstChildElement("Faces");
		objElement->QueryIntAttribute("vertexOffset", &vertexOffset);
		str = objElement->GetText();
		while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
			cursor++;
		while(str[cursor] != '\0')
		{
			for(int cnt = 0 ; cnt < 3 ; cnt++)
			{
				if(cnt == 0)
					p1Index = atoi(str + cursor) + vertexOffset;
				else if(cnt == 1)
					p2Index = atoi(str + cursor) + vertexOffset;
				else
					p3Index = atoi(str + cursor) + vertexOffset;
				while(str[cursor] != ' ' && str[cursor] != '\t' && str[cursor] != '\n')
					cursor++; 
				while(str[cursor] == ' ' || str[cursor] == '\t' || str[cursor] == '\n')
					cursor++;
			}
			faces.push_back(*(new Triangle(-1, matIndex, p1Index, p2Index, p3Index)));
		}

		objects.push_back(new Mesh(id, matIndex, faces));

		pObject = pObject->NextSiblingElement("Mesh");
	}

	// Parse lights
	int id;
	Vector3f position;
	Vector3f intensity;
	pElement = pRoot->FirstChildElement("Lights");

	XMLElement *pLight = pElement->FirstChildElement("AmbientLight");
	XMLElement *lightElement;
	str = pLight->GetText();
	sscanf(str, "%f %f %f", &ambientLight.r, &ambientLight.g, &ambientLight.b);

	pLight = pElement->FirstChildElement("PointLight");
	while(pLight != nullptr)
	{
		eResult = pLight->QueryIntAttribute("id", &id);
		lightElement = pLight->FirstChildElement("Position");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &position.x, &position.y, &position.z);
		lightElement = pLight->FirstChildElement("Intensity");
		str = lightElement->GetText();
		sscanf(str, "%f %f %f", &intensity.r, &intensity.g, &intensity.b);

		lights.push_back(new PointLight(position, intensity));

		pLight = pLight->NextSiblingElement("PointLight");
	}
}

