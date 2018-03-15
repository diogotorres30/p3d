#ifndef __TRIANGLE_H__
#define __TRIANGLE_H__

#include "Mesh.h"

class Triangle : public Mesh
{
private:
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 normal;

public:
	Triangle();
	Triangle(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat);
	float intersect(Ray &ray);
	Vector3 getNormal(Ray &ray);

};

#endif /* __TRIANGLE_H__ */