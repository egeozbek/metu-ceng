#include "Camera.h"

Camera::Camera(int id,                      // Id of the camera
               const char* imageName,       // Name of the output PPM file 
               const Vector3f& pos,         // Camera position
               const Vector3f& gaze,        // Camera gaze direction
               const Vector3f& up,          // Camera up direction
               const ImagePlane& imgPlane)  // Image plane parameters
     : id(id),
          imgPlane(imgPlane),
          pos(pos),
          gaze(gaze),
          up(up)
{ 
     for(int i = 0 ; i<32 ; i++){
          this->imageName[i] = imageName[i];
     }
     distanceBetweenColumns = (imgPlane.right - imgPlane.left)/imgPlane.nx;
     distanceBetweenRows = (imgPlane.top - imgPlane.bottom)/imgPlane.ny;
     Vector3f wVector = vectorUtils.vectorMultiplication(gaze,-1);
     crossedVector = vectorUtils.crossProduct(up,wVector);

	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
}

/* Takes coordinate of an image pixel as row and col,{horizontalCoordinate,verticalCoordinate,-imgPlane.distance};
 * returns the ray going through that pixel. 
 */
Ray Camera::getPrimaryRay(int col, int row) const
{
	/***********************************************{horizontalCoordinate,verticalCoordinate,-imgPlane.distance};
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */
      //find the origin of image plane
     Vector3f gazedVector = vectorUtils.vectorMultiplication(gaze,imgPlane.distance);
     Vector3f pointOnPlane = vectorUtils.vectorAddition(pos,gazedVector);

     Vector3f toLeftOfPlaneCoefficient = vectorUtils.vectorMultiplication(crossedVector ,imgPlane.left);
     Vector3f toTopOfPlaneCoefficient = vectorUtils.vectorMultiplication(up,imgPlane.top);

     Vector3f toLeftOfPlaneSummed = vectorUtils.vectorAddition(pointOnPlane,toLeftOfPlaneCoefficient);
     Vector3f topLeftOfPlane = vectorUtils.vectorAddition(toLeftOfPlaneSummed,toTopOfPlaneCoefficient);

     //TODO not sure about col and row
     float horizontalCoordinate =  (row + 0.5) * (distanceBetweenColumns);
     float verticalCoordinate = (col + 0.5) * (distanceBetweenRows);

     Vector3f correctPlaceToLeft = vectorUtils.vectorMultiplication(crossedVector,horizontalCoordinate);
     Vector3f correctPlaceToTop = vectorUtils.vectorMultiplication(up,verticalCoordinate);
     

     Vector3f firstSumVector = vectorUtils.vectorAddition(topLeftOfPlane,correctPlaceToLeft);
     Vector3f finalSumVector = vectorUtils.vectorSubtraction(firstSumVector,correctPlaceToTop);

     Vector3f directionVector = vectorUtils.vectorSubtraction(finalSumVector,pos);

     Ray primaryRay (pos,directionVector);
     return primaryRay;
}

