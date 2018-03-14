#include "Sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(Vector3 &newcenter, float newradius, Material *mat) : Mesh(mat)
{
    center = newcenter;
    radius = newradius;
}

float Sphere::intersect(Ray &ray){
    float t = -1.0;
    float radius2 = radius * radius;
    Vector3 dist = center - ray.origin;
    dist2 = quadrance(dist);
    float b = dot(ray.direction, dist);
    
    if(dist2 > radius2 && b < 0.0f)
        return t;
    
    float R = (b * b) - dist2 + radius2;
    
    if(R < 0.0f)
        return t;
        
    if (dist2 > radius2)
        t = b - sqrt(R);
    else
        t = b + sqrt(R);
    
    ray.point.x = ray.origin.x + ( ray.direction.x * t );
    ray.point.y = ray.origin.y + ( ray.direction.y * t );
    ray.point.z = ray.origin.z + ( ray.direction.z * t );
    
    return t;
}

Vector3 Sphere::getNormal(Ray &ray)
{
	if (dist2 < (radius*radius))
	{
		return Vector3(-(ray.point.x - center.x) / radius, -(ray.point.y - center.y) / radius, -(ray.point.z - center.z) / radius);
	}
	else
	{
		return Vector3((ray.point.x - center.x) / radius, (ray.point.y - center.y) / radius, (ray.point.z - center.z) / radius);
	}
}
