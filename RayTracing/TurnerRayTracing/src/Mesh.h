#ifndef __MESH_H__
#define __MESH_H__

#include <GL/glew.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include "MathAux.h"
#include "Material.h"
#include "Ray.h"

class Mesh
{
    public:
        Material* material;

    public:
        Mesh();
        Mesh(Material* mat);
        virtual float intersect(Ray r) = 0;

};

#endif // !__MESH_H__
