#include "Sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(Vector3 &newcenter, float newradius, Material *mat) : Mesh(mat)
{
    center = newcenter;
    radius = newradius;
}

float Sphere::intersect(Ray r){
    
    return 0;
}
