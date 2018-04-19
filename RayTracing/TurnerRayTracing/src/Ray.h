#ifndef __RAY_H__
#define __RAY_H__

#include "MathAux.h"
#include "Camera.h"

class Ray
{
	public:
		int ID;
		Vector3 origin, point;
		Vector3 direction;
		float scalar;
		Ray();
		Ray(Camera *camera, Vector2 pt, int uniqueID);
		Ray(Vector3 orig, Vector3 dir, int uniqueID);
		~Ray();
		void intersectionPoint(float t);
    
};


#endif // !__RAY_H__

