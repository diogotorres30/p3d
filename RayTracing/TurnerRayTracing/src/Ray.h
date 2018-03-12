#ifndef __RAY_H__
#define __RAY_H__

#include "MathAux.h"

class Ray
{
	public:
		Vector3 origin, point;
		Vector3 direction;
		float scalar;
		Ray();
		Ray(Vector3 org, Vector2 pt);
		~Ray();
    
};


#endif // !__RAY_H__

