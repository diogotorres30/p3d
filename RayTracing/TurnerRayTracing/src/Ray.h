#ifndef __RAY_H__
#define __RAY_H__

#include "MathAux.h"

class Ray
{
public:
	Vector3 origin, point;
	Vector3 direction;
	float scalar;
};


#endif // !__RAY_H__

