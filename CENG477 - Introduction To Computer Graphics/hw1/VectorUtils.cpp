#include "VectorUtils.h"

float VectorUtils::dotProduct(const Vector3f & first , const Vector3f & second) const{
    return first.x * second.x +
            first.y * second.y +
            first.z * second.z;
}

Vector3f VectorUtils::crossProduct(const Vector3f & first , const Vector3f & second) const
{
    Vector3f resultVector;
    resultVector.x = (first.y * second.z) - (first.z * second.y);
    resultVector.y = (first.z * second.x) - (first.x * second.z);
    resultVector.z = (first.x * second.y) - (first.y * second.x);
    
    return resultVector;

    
}

Vector3f VectorUtils::vectorAddition(const Vector3f & first , const Vector3f & second) const
{
    Vector3f resultVector;
        resultVector.x = first.x + second.x;
        resultVector.y = first.y + second.y;
        resultVector.z = first.z + second.z;
    return resultVector;
}
Vector3f VectorUtils::vectorSubtraction(const Vector3f & first , const Vector3f & second) const{
    Vector3f resultVector;
        resultVector.x = first.x - second.x;
        resultVector.y = first.y - second.y;
        resultVector.z = first.z - second.z;
    return resultVector;
}


Vector3f VectorUtils::vectorMultiplication(const Vector3f & originalVector , const float scalar)const
{

    Vector3f returnVector;
        returnVector.x = originalVector.x * scalar;
        returnVector.y = originalVector.y * scalar;
        returnVector.z = originalVector.z * scalar;
    return returnVector;
}
Vector3f VectorUtils::vectorDivision(const Vector3f & originalVector , const float scalar)const
{
    Vector3f returnVector;
        returnVector.x = originalVector.x / scalar;
        returnVector.y = originalVector.y / scalar;
        returnVector.z = originalVector.z / scalar;
    return returnVector;
}
Vector3f VectorUtils::vectorElementwiseMultiplication(const Vector3f & firstVector , const Vector3f & secondVector)const
{
    Vector3f returnVector;
        returnVector.x = firstVector.x * secondVector.x;
        returnVector.y = firstVector.y * secondVector.y;
        returnVector.z = firstVector.z * secondVector.z;
    return returnVector;
}
Vector3f VectorUtils::vectorElementwiseDivision(const Vector3f & firstVector , const Vector3f & secondVector)const{
    Vector3f returnVector;
        returnVector.x = firstVector.x / secondVector.x;
        returnVector.y = firstVector.y / secondVector.y;
        returnVector.z = firstVector.z / secondVector.z;
    return returnVector;

}


float VectorUtils :: crammerMethod(const Vector3f & firstColumn , const Vector3f & secondColumn , const Vector3f & thirdColumn ) const{
    return firstColumn.x * ((secondColumn.y * thirdColumn.z) - (thirdColumn.y * secondColumn.z) ) +
           firstColumn.y * ((thirdColumn.x * secondColumn.z) - (secondColumn.x * thirdColumn.z) ) +
           firstColumn.z * ((secondColumn.x * thirdColumn.y) - (secondColumn .y * thirdColumn.x) );
}

void VectorUtils :: printVector(const Vector3f & vector) const {
     std::cout<<"vector.x: "<< vector.x <<" vector.y: "<< vector.y <<" vector.z: "<< vector.z <<std::endl;
}

Vector3f VectorUtils :: normalizeVector(const Vector3f & vector) const {
    float vectorLength = sqrt(
        pow(vector.x,2.0) +
        pow(vector.y,2.0) +
        pow(vector.z,2.0)
    );
    Vector3f normal = {
        vector.x / vectorLength,
        vector.y / vectorLength,
        vector.z / vectorLength,
    };
    return normal;
}

    bool VectorUtils :: isVectorAllZeros(const Vector3f & vector) const{
        if(vector.x == 0.0 && vector.y == 0.0 && vector.z == 0.0 ){
            return true;
        }
        return false;
    }



Color VectorUtils:: vecToColorWithClamp (const Vector3f & inputVector) const{
    Color returnColor;

    returnColor.red = inputVector.r < 255. ? static_cast<unsigned char> (inputVector.r) : 255.;
	returnColor.grn = inputVector.g < 255. ? static_cast<unsigned char> (inputVector.g) : 255.;
	returnColor.blu = inputVector.b < 255. ? static_cast<unsigned char> (inputVector.b) : 255.;

    //TODO THIS Gives weird outputs
    
    //std::cout<<"After clamping returnColor.x: "<< returnColor.red <<" returnColor.y: "<< returnColor.grn <<" colorTrio.z: "<< returnColor.blu <<std::endl;


    return returnColor;
}


