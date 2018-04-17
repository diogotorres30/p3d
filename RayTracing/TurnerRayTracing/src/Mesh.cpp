#include "Mesh.h"

Mesh::Mesh(){}

Mesh::Mesh(Material* mat)
{
    material = mat;
}

float Mesh::intersect(Ray &ray)
{
	return 0.0f;
}

Material *Mesh::getMaterial()
{
	return material;
}

BoudingBox *Mesh::getBoudingBox()
{
	return boudingBox;
}

void Mesh::setBoudingBox(BoudingBox* box)
{
	boudingBox = box;
}