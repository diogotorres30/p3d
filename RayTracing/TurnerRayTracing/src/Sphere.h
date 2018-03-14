#ifndef Sphere_h
#define Sphere_h

#include "Mesh.h"

class Sphere : public Mesh
{
    private:
        float radius, dist2;
        Vector3 center;
    
    public:
        Sphere();
        Sphere(Vector3 &newcenter, float radius, Material *mat);
        float intersect(Ray &ray);
		Vector3 getNormal(Ray &ray);
};


#endif /* Sphere_h */
