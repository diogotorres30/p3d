#ifndef Plane_h
#define Plane_h

#include "Mesh.h"

class Plane : public Mesh
{
    private:
        Vector3 p1;
        Vector3 p2;
        Vector3 p3;
		Vector3 normal;
    
    public:
        Plane();
        Plane(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat);
        float intersect(Ray &ray);
		Vector3 getNormal(Ray &ray);
    
};

#endif /* Plane_h */
