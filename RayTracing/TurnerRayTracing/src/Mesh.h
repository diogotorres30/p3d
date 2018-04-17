#ifndef __MESH_H__
#define __MESH_H__

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

class Mesh
{
    private:
        Material* material;

    public:
        Mesh();
        Mesh(Material* mat);
		Material *getMaterial();
        virtual float intersect(Ray &ray) = 0;
		virtual Vector3 getNormal(Ray &ray) = 0;

};

#endif // !__MESH_H__
