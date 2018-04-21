#ifndef BoundingBox_h
#define BoundingBox_h

#include <stdio.h>

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <math.h>
#include "MathAux.h"
#include "Material.h"
#include "Ray.h"

class BoundingBox
{
    private:
        Vector3 normal;
        float xMax, yMax, zMax;
        float xMin, yMin, zMin;

    public:
        BoundingBox();
        BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
        float intersect(Ray &ray);
		Vector2 intersectTwo(Ray &ray);
		float getXMin() { return xMin; };
		float getXMax() { return xMax; };
		float getYMin() { return yMin; };
		float getYMax() { return yMax; };
		float getZMin() { return zMin; };
		float getZMax() { return zMax; };
        Vector3 getNormal(Ray &ray);
};


#endif /* BoundingBox_h */
