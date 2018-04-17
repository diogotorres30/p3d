#ifndef Box_h
#define Box_h

#include "Mesh.h"

class Box : public Mesh
{
    private:
        Vector3 normal;
        float xMax, yMax, zMax;
        float xMin, yMin, zMin;
		Mesh* object;

    public:
        Box();
        Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
        float intersect(Ray &ray);
        Vector3 getNormal(Ray &ray);
};


#endif /* Box_hp */
