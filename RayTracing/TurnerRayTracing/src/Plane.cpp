#include "Plane.h"

Plane::Plane(){}

Plane::Plane(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat) : Mesh(mat)
{
    p1 = newp1;
    p2 = newp2;
    p3 = newp3;
}

float Plane::intersect(Ray r){
    
    return 0;
}
