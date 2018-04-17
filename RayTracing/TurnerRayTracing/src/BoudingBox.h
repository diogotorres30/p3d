#ifndef BoudingBox_h
#define BoudingBox_h

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

class BoudingBox
{
    private:
        Vector3 normal;
        float xMax, yMax, zMax;
        float xMin, yMin, zMin;

    public:
        BoudingBox();
        BoudingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
        float intersect(Ray &ray);
        Vector3 getNormal(Ray &ray);
};


#endif /* BoudingBox_h */
