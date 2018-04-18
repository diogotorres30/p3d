#ifndef Box_h
#define Box_h

#include "Mesh.h"
#include <vector>

class Box : public Mesh
{
    private:
		std::vector<Mesh*> *meshes = new std::vector<Mesh*>();
        Vector3 normal;
        float xMax, yMax, zMax;
        float xMin, yMin, zMin;


    public:
        Box();
        Box(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
		float getXMin() { return xMin; };
		float getXMax() { return xMax; };
		float getYMin() { return yMin; };
		float getYMax() { return yMax; };
		float getZMin() { return zMin; };
		float getZMax() { return zMax; };
		std::vector<Mesh*> *getMeshes() { return meshes; };
        float intersect(Ray &ray);
        Vector3 getNormal(Ray &ray);
};


#endif /* Box_hp */
