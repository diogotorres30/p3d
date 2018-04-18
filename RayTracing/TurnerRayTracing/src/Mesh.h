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
#include "BoundingBox.h"
#include "Ray.h"

class Mesh
{
private:
	Material * material;
	BoundingBox* boundingBox = nullptr;
	float xMax, yMax, zMax;
	float xMin, yMin, zMin;

public:
	Mesh();
	Mesh(Material* mat);
	Material *getMaterial();
	BoundingBox *getBoundingBox();
	void setBoundingBox(BoundingBox* box);
	virtual float intersect(Ray &ray) = 0;
	virtual Vector3 getNormal(Ray &ray) = 0;
};

#endif // !__MESH_H__
