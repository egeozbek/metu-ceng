#ifndef _VECTORUTILS_H_
#define _VECTORUTILS_H_

#include "defs.h"
#include "Image.h"

class VectorUtils
{
public:
    float dotProduct(const Vector3f & first ,const Vector3f & second) const ;
    Vector3f crossProduct(const Vector3f & first , const Vector3f & second) const;
    Vector3f vectorAddition(const Vector3f & first , const Vector3f & second) const;
    Vector3f vectorSubtraction(const Vector3f & first , const Vector3f & second) const;
	Vector3f vectorMultiplication(const Vector3f & originalVector , const float scalar) const;
	Vector3f vectorDivision(const Vector3f & originalVector , const float scalar) const;
	Vector3f vectorElementwiseMultiplication(const Vector3f & firstVector , const Vector3f & secondVector) const;
	Vector3f vectorElementwiseDivision(const Vector3f & firstVector , const Vector3f & secondVector) const;
	float crammerMethod(const Vector3f & firstColumn , const Vector3f & secondColumn , const Vector3f & thirdColumn ) const;
    void printVector(const Vector3f & vector)const;
    Vector3f normalizeVector(const Vector3f & vector) const;
    bool isVectorAllZeros(const Vector3f & vector) const;
    Color vecToColorWithClamp (const Vector3f & inputVector) const;
    float getDistanceBetweenTwoPoints(const Vector3f & first,const Vector3f & second) const;

};
#endif