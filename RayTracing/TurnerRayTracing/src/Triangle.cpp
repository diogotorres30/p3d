#include "Triangle.h"

Triangle::Triangle() {}

Triangle::Triangle(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat) : Mesh(mat)
{
	p1 = newp1;
	p2 = newp2;
	p3 = newp3;
}

float Triangle::intersect(Ray &ray) {
	float t = -1.0;


	return t;
}

Vector3 Triangle::getNormal(Ray &ray)
{
	return normal;
}