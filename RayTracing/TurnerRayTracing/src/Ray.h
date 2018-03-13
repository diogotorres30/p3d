#ifndef __RAY_H__
#define __RAY_H__

#include "MathAux.h"
#include "Camera.h"

class Ray
{
	public:
		Vector3 origin, point;
		Vector3 direction;
		float scalar;
		Ray();
		Ray(Camera *camera, Vector2 pt);
		~Ray();
    
};


#endif // !__RAY_H__

