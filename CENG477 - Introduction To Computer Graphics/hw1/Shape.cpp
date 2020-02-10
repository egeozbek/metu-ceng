#include "Shape.h"
#include "Scene.h"
#include <cstdio>

Shape::Shape(void)
{
}

Shape::Shape(int id, int matIndex)
    : id(id), matIndex(matIndex)
{
}


Sphere::Sphere(void)
{}

/* Constructor for sphere. You will implement this. */
Sphere::Sphere(int id, int matIndex, int cIndex, float R)
    : Shape(id, matIndex), centerCoordinateIndex(cIndex), sphereRadius(R)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
     
}

Vector3f Sphere::getCenterCoordinates()const{
    return pScene->vertices[centerCoordinateIndex-1];
}
float Sphere::getDistanceBetweenTwoPoints(const Vector3f & first,const Vector3f & second) const{
	return sqrt(
		pow(first.x - second.x,2.0) +
		pow(first.y - second.y,2.0) +
		pow(first.z - second.z,2.0)
	);
}

/* Sphere-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Sphere::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    const Vector3f centerCoordinates = getCenterCoordinates();
    Vector3f originToCenter = vectorUtils.vectorSubtraction(ray.origin,centerCoordinates);

    float firstPartDot = vectorUtils.dotProduct(ray.direction,originToCenter) ;

    float firstPartSquared = pow(firstPartDot,2.0);

    float directionSquared = vectorUtils.dotProduct(ray.direction,ray.direction);
    float originToCenterSquared = vectorUtils.dotProduct(originToCenter,originToCenter);

    float rightPart = originToCenterSquared - pow(sphereRadius,2.0);
    float rightmostPart = directionSquared * rightPart;

    float determinant = firstPartSquared - rightmostPart;

    ReturnVal intersectionValues = {false,
    		-1,
            {0,0,0},
            {0,0,0}
            };

    if(determinant < 0.0){
        //std::cout<<"Determinant < 0"<<std::endl;
        return intersectionValues;
    }
    else if (determinant == 0.0){
        //std::cout<<"Determinant = 0"<<std::endl;
        float t = - vectorUtils.dotProduct(originToCenter,ray.direction);
        t = t / directionSquared ;

        if (t < pScene->intTestEps){
            return intersectionValues;
        }
        else{
            intersectionValues.isIntersected = true;
            intersectionValues.materialId = matIndex;

            Vector3f intersectionPoint = ray.getPoint(t);
            Vector3f normalVector = vectorUtils.vectorSubtraction(intersectionPoint,centerCoordinates);
            Vector3f unitNormalVector = vectorUtils.normalizeVector(normalVector);

            intersectionValues.intersectionPoint = intersectionPoint;
            intersectionValues.unitSurfaceNormal = unitNormalVector;
        }
    }
    else{
        //std::cout<<"Determinant > 0"<<std::endl;
        float sqrtDeterminant = sqrt(determinant);
        float angleInBetween = - vectorUtils.dotProduct(originToCenter,ray.direction);
        float t1Value = (angleInBetween + sqrtDeterminant)/directionSquared;
        float t2Value = (angleInBetween - sqrtDeterminant)/directionSquared;

        if (t1Value < pScene->intTestEps){
            return intersectionValues;
        }
        Vector3f intersectionPoint1 = ray.getPoint(t1Value);
        Vector3f intersectionPoint2 = ray.getPoint(t2Value);

        float distanceToIntersection1 = getDistanceBetweenTwoPoints(ray.origin,intersectionPoint1);
        float distanceToIntersection2 = getDistanceBetweenTwoPoints(ray.origin,intersectionPoint2);

        Vector3f intersectionPoint;

        if(distanceToIntersection1 < distanceToIntersection2){
            intersectionPoint = intersectionPoint1;
        }
        else{
            intersectionPoint = intersectionPoint2;
        }
    
        Vector3f normalVector = vectorUtils.vectorSubtraction(intersectionPoint,centerCoordinates);
        Vector3f unitNormalVector  = vectorUtils.normalizeVector(normalVector);
        //unit vectoru normalize etmezsen saçmalıyor

        intersectionValues.isIntersected = true;
        intersectionValues.materialId = matIndex;
        intersectionValues.intersectionPoint = intersectionPoint;
        intersectionValues.unitSurfaceNormal = unitNormalVector;

       
    }
    return intersectionValues;
    
}

Triangle::Triangle(void)
{}

/* Constructor for triangle. You will implement this. */
Triangle::Triangle(int id, int matIndex, int p1Index, int p2Index, int p3Index)
    : Shape(id, matIndex),firstVertexIndex(p1Index),secondVertexIndex(p2Index),thirdVertexIndex(p3Index)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

Vector3f Triangle::getFirstVertexCoordinates() const{
    return pScene->vertices[firstVertexIndex-1];
}

Vector3f Triangle::getSecondVertexCoordinates() const{
    return pScene->vertices[secondVertexIndex-1];
}

Vector3f Triangle::getThirdVertexCoordinates() const{
    return pScene->vertices[thirdVertexIndex-1];
}

/* Triangle-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Triangle::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    Vector3f firstCoordinates = getFirstVertexCoordinates();
    Vector3f secondCoordinates = getSecondVertexCoordinates();
    Vector3f thirdCoordinates = getThirdVertexCoordinates();

    ReturnVal returnValues = {
        false,
        -1,
        {0,0,0},
        {0,0,0}
    };

    float determinant = vectorUtils.crammerMethod(
                        vectorUtils.vectorSubtraction(firstCoordinates,secondCoordinates),
                        vectorUtils.vectorSubtraction(firstCoordinates,thirdCoordinates),
                        ray.direction
                        );

    float betaUpper = vectorUtils.crammerMethod(
        vectorUtils.vectorSubtraction(firstCoordinates,ray.origin),
        vectorUtils.vectorSubtraction(firstCoordinates,thirdCoordinates),
        ray.direction
    );
    float gammaUpper = vectorUtils.crammerMethod(
        vectorUtils.vectorSubtraction(firstCoordinates,secondCoordinates),
        vectorUtils.vectorSubtraction(firstCoordinates,ray.origin),
        ray.direction
    );
    float tUpper = vectorUtils.crammerMethod(
        vectorUtils.vectorSubtraction(firstCoordinates,secondCoordinates),
        vectorUtils.vectorSubtraction(firstCoordinates,thirdCoordinates),
        vectorUtils.vectorSubtraction(firstCoordinates,ray.origin)
    );
    float beta = betaUpper / determinant;
    float gamma = gammaUpper / determinant;
    float t = tUpper / determinant;

    if (gamma < 0 || gamma > 1) {
        return returnValues;
    }
    if (beta < 0 || beta > (1 - gamma)) {
        return returnValues;
    }
    if (t < (pScene->intTestEps)){
        return returnValues;
    }

    /*

    if(gamma + beta > 1.0){
        return returnValues;
    }
    if(beta < 0.0 ){
        return returnValues;
    }
    if(gamma < 0.0 ){
        return returnValues;
    }
    */
    Vector3f BMinusA = vectorUtils.vectorSubtraction(secondCoordinates,firstCoordinates);
    Vector3f CMinusA = vectorUtils.vectorSubtraction(thirdCoordinates,firstCoordinates);
    Vector3f surfaceNormal = vectorUtils.crossProduct(BMinusA,CMinusA);
    surfaceNormal = vectorUtils.normalizeVector(surfaceNormal);

    /*
    float howParallelAreThey = vectorUtils.dotProduct(surfaceNormal,ray.direction);

    if(abs(howParallelAreThey) < pScene->intTestEps){
        return returnValues;
    }
    */
    float cosineBetweenRayAndNormal = vectorUtils.dotProduct(ray.direction,surfaceNormal);
    bool isTriangleFacedIncorrectly = cosineBetweenRayAndNormal > 0 ? true : false ;
    if(isTriangleFacedIncorrectly){
        surfaceNormal = vectorUtils.vectorMultiplication(surfaceNormal,-1);
    }
    returnValues.isIntersected = true;

    Vector3f intersectionDirection = vectorUtils.vectorMultiplication(ray.direction,t);
    Vector3f intersectionPoint = vectorUtils.vectorAddition(ray.origin,intersectionDirection);

    returnValues.materialId = matIndex ;
    returnValues.intersectionPoint = intersectionPoint;
    returnValues.unitSurfaceNormal = surfaceNormal;
    
    return returnValues;

}

Mesh::Mesh()
{}

/* Constructor for mesh. You will implement this. */
Mesh::Mesh(int id, int matIndex, const vector<Triangle>& faces)
    : Shape(id, matIndex)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    triangleFaces = faces;
}

float Mesh::getDistanceBetweenTwoPoints(const Vector3f & first,const Vector3f & second) const{
	return sqrt(
		pow(first.x - second.x,2.0) +
		pow(first.y - second.y,2.0) +
		pow(first.z - second.z,2.0)
	);
}

/* Mesh-ray intersection routine. You will implement this. 
Note that ReturnVal structure should hold the information related to the intersection point, e.g., coordinate of that point, normal at that point etc. 
You should to declare the variables in ReturnVal structure you think you will need. It is in defs.h file. */
ReturnVal Mesh::intersect(const Ray & ray) const
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
    ReturnVal realReturnValue = {
        false,
        -1,
        {0,0,0},
        {0,0,0}
    };
    int numberOfTriangles = triangleFaces.size();
    float minIntersectionDistance = std::numeric_limits<float>::max();

    for(int i = 0 ; i < numberOfTriangles ; i++){
        ReturnVal currentIntersection = triangleFaces[i].intersect(ray);
        if(currentIntersection.isIntersected){
            realReturnValue.isIntersected = true;
            //TODO getDistanceBetweenTwoPoints
            float currentIntersectionDistance = getDistanceBetweenTwoPoints(ray.origin,currentIntersection.intersectionPoint);
            if( currentIntersectionDistance < minIntersectionDistance ){
                minIntersectionDistance = currentIntersectionDistance;
                realReturnValue.materialId = matIndex;
                realReturnValue.intersectionPoint = currentIntersection.intersectionPoint;
                realReturnValue.unitSurfaceNormal = currentIntersection.unitSurfaceNormal;
            }
        }
    }
    return realReturnValue;
}
