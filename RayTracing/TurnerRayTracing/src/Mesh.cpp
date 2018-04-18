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

BoundingBox *Mesh::getBoundingBox()
{
	return boundingBox;
}

void Mesh::setBoundingBox(BoundingBox* box)
{
	boundingBox = box;
}