#ifndef Box_h
#define Box_h

#include "Mesh.h"

class Box : public Mesh
{
    private:
        Vector3 normal;
        float xMax, yMax, zMax;
        float xMin, yMin, zMin;

    public:
        Box();
        Box(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
        float intersect(Ray &ray);
        Vector3 getNormal(Ray &ray);
};


#endif /* Box_hp */
