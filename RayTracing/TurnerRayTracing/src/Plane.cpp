#include "Plane.h"

Plane::Plane(){}

Plane::Plane(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat) : Mesh(mat)
{
    p1 = newp1;
    p2 = newp2;
    p3 = newp3;
}

float Plane::intersect(Ray &ray){
    float t = -1.0;
    Vector3 ab = p2 - p1;
    Vector3 ac = p3 - p1;
    normal = cross(ab, ac);
    float nd = dot(normal, ray.direction);
    
    if(nd == 0)
        return t;
    
	t = -1 * (dot((ray.origin - p1), normal) / nd);

	ray.point.x = ray.origin.x + (ray.direction.x * t);
	ray.point.y = ray.origin.y + (ray.direction.y * t);
	ray.point.z = ray.origin.z + (ray.direction.z * t);
    
    return t;
}

Vector3 Plane::getNormal(Ray &ray)
{
	return normal;
}