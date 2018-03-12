#include "Plane.h"

Plane::Plane(){}

Plane::Plane(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat) : Mesh(mat)
{
    p1 = newp1;
    p2 = newp2;
    p3 = newp3;
}

float Plane::intersect(Ray ray){
    float t = -1.0;
    Vector3 ab = p2 - p1;
    Vector3 ac = p3 - p1;
    Vector3 normal = cross(ab, ac);
    float nd = dot(normal, ray.direction);
    
    if(nd == 0)
        return t;
    
    float d = dot(normal, p1);
    t = -1 * ((dot(normal, ray.origin) + d) / (nd));
    
    if(t < 0)
        return -1.0;
    
    return t;
}
