#include "Light.h"

/* Constructor. Implemented for you. */
PointLight::PointLight(const Vector3f & position, const Vector3f & intensity)
    : position(position), intensity(intensity)
{
}

// Compute the contribution of light at point p using the
// inverse square law formula
Vector3f PointLight::computeLightContribution(const Vector3f& p)
{
	/***********************************************
     *                                             *
	 * TODO: Implement this function               *
     *                                             *
     ***********************************************
	 */

    float distanceSquared =
        pow(p.x-position.x,2.0)+
        pow(p.y-position.y,2.0)+
        pow(p.z-position.z,2.0);
         
     Vector3f contributions = {
         intensity.x/distanceSquared,
         intensity.y/distanceSquared,
         intensity.z/distanceSquared
         };
        
     return contributions;
}
