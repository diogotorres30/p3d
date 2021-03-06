#include "Ray.h"

Ray::Ray(){}

Ray::Ray(Camera *camera, Vector2 pt, int uniqueID)
{
	point = Vector3(0.0f);
	origin = camera->getFrom();
	
	Vector3 z = -(camera->getDf()) * camera->getZe();
	Vector3 y = camera->getHeight() * (((pt.y + 0.5f)/ camera->getResY()) - 0.5f) * camera->getYe();
	Vector3 x = camera->getWidth() * (((pt.x + 0.5f) / camera->getResX()) - 0.5f) * camera->getXe();

	direction = normalized(z + y + x);
	scalar = 0.0f;
	ID = uniqueID;
}

Ray::Ray(Vector3 orig, Vector3 dir, int uniqueID)
{
	point = Vector3(0.0f);
	origin = orig;
	direction = normalized(dir);
	scalar = 0.0f;
	ID = uniqueID;
}

Ray::~Ray(){}

void Ray::intersectionPoint(float t)
{
	point = origin + (direction * t);
}