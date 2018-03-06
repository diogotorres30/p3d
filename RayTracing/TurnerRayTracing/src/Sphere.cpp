#include "Sphere.h"

Sphere::Sphere(){}

Sphere::Sphere(Vector3 &newcenter, float newradius)
{
    center = newcenter;
    radius = newradius;
}
