#include "Triangle.h"

Triangle::Triangle() {}

Triangle::Triangle(Vector3 newp1, Vector3 newp2, Vector3 newp3, Material *mat) : Mesh(mat)
{
	p1 = newp1;
	p2 = newp2;
	p3 = newp3;

	float xMin, xMax, yMin, yMax, zMin, zMax;

	xMin = std::min(p1.x, p2.x);
	xMin = std::min(xMin, p3.x);

	xMax = std::max(p1.x, p2.x);
	xMax = std::max(xMax, p3.x);

	yMin = std::min(p1.y, p2.y);
	yMin = std::min(yMin, p3.y);

	yMax = std::max(p1.y, p2.y);
	yMax = std::max(yMax, p3.y);

	zMin = std::min(p1.z, p2.z);
	zMin = std::min(zMin, p3.z);

	zMax = std::max(p1.z, p2.z);
	zMax = std::max(zMax, p3.z);

	BoundingBox* box = new BoundingBox(xMin, xMax, yMin, yMax, zMin, zMax);
	setBoundingBox(box);
}

float Triangle::intersect(Ray &ray) {

	if (ray.ID == getCachedRayId())
	{
		return getCachedIntersection();
	}
	else
	{
		const float epsilon = 0.0000001;
		Vector3 edge1, edge2, h, s, q;
		float t = -1.0;
		float a, f, u, v;
		edge1 = p2 - p1;
		edge2 = p3 - p1;
		normal = cross(edge1, edge2);
		h = cross(ray.direction, edge2);
		a = dot(edge1, h);
		if (a > -epsilon && a < epsilon)
			return t;
		f = 1 / a;
		s = ray.origin - p1;
		u = f * dot(s, h);
		if (u < 0.0f || u > 1.0f)
			return t;
		q = cross(s, edge1);
		v = f * dot(ray.direction, q);
		if (v < 0.0f || u + v > 1.0f)
			return t;
		t = f * dot(edge2, q);
		if (t < epsilon)
			return -1.0f;

		setCachedRayId(ray.ID);
		setCachedIntersection(t);

		return t;
	}
}

Vector3 Triangle::getNormal(Ray &ray)
{
	return normal;
}
