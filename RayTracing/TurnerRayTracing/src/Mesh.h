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

class Mesh
{
    public:
        Material* material;

    public:
        Mesh();
        Mesh(Material* mat);

};

#endif // !__MESH_H__
