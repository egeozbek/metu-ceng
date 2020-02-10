#ifndef _SCENE_H_
#define _SCENE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include "Ray.h"
#include "defs.h"
#include "Image.h"
#include <limits>
#include "VectorUtils.h"


// Forward declarations to avoid cyclic references
class Camera;
class PointLight;
class Material;
class Shape;

using namespace std;

// Class to hold everything related to a scene. 
class Scene
{
public:
	int maxRecursionDepth;			// Maximum recursion depth
	float intTestEps;				// IntersectionTestEpsilon. You will need this one while implementing intersect routines in Shape class
	float shadowRayEps;				// ShadowRayEpsilon. You will need this one while generating shadow rays. 
	Vector3f backgroundColor;		// Background color
	Vector3f ambientLight;			// Ambient light radiance

	vector<Camera *> cameras;		// Vector holding all cameras
	vector<PointLight *> lights;	// Vector holding all point lights
	vector<Material *> materials;	// Vector holding all materials
	vector<Vector3f> vertices;		// Vector holding all vertices (vertex data)
	vector<Shape *> objects;		// Vector holding all shapes

	Scene(const char *xmlPath);		// Constructor. Parses XML file and initializes vectors above. Implemented for you. 

	void renderScene(void);			// Method to render scene, an image is created for each camera in the scene. You will implement this. 

private:
    // Write any other stuff here
	VectorUtils vectorUtils;
	ReturnVal traceRay(const Ray & ray, const int recursionLeft)const;
    ReturnVal traceShadowRay(const Ray & ray)const;
    Color getBackgroundColor();
	Vector3f ambientLightningColor(int materialId) const;
	float getDistanceBetweenTwoPoints(const Vector3f & first,const Vector3f & second) const;
	Vector3f diffuseLightningColor(const Vector3f & diffuseReflectionCoefficients, PointLight * currentLightSource, const Vector3f & intersectionPoint , const Vector3f & directionToLightSourceNormalized , const Vector3f & unitNormal) const;
	Vector3f specularLightningColor(const Vector3f & specularReflectionCoefficients, int phongExponent , PointLight * currentLightSource, const Vector3f & intersectionPoint,const Vector3f & vectorToCamera, const Vector3f &  directionToLightSourceNormalized, const Vector3f & unitNormal) const;
	Vector3f getLocalColor(int materialId , const Vector3f & intersectionPoint , const Vector3f & unitNormal, const Vector3f & cameraOrigin) const;

};

#endif
