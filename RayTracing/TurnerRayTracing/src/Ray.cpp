#include "Ray.h"

Ray::Ray(){}

Ray::Ray(Vector3 org, Vector2 pt)
{
	origin = org;
	point = Vector3(pt, 0.0f);
	direction = normalized(point - origin);
	scalar = 0;
}

Ray::~Ray(){}


float Ray::ray_sphere(Mesh *mesh)
{
    
    
    return 0.0;
    
}
