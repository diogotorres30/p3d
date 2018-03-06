#ifndef Sphere_h
#define Sphere_h

#include "Mesh.h"

class Sphere : public Mesh
{
    private:
        float radius;
        Vector3 center;
    
    public:
        Sphere();
        Sphere(Vector3 &newcenter, float radius, Material *mat);
    
};


#endif /* Sphere_h */
