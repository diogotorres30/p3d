#include "Ray.h"

Ray::Ray(){}

Ray::Ray(Camera *camera, Vector2 pt)
{
	point = Vector3(0.0f);
	origin = camera->getFrom();
	
	Vector3 z = -(camera->getDf()) * camera->getZe();
	Vector3 y = camera->getHeight() * ((pt.y / camera->getResY()) - 0.5) * camera->getYe();
	Vector3 x = camera->getWidth() * ((pt.x / camera->getResX()) - 0.5) * camera->getXe();

	direction = normalized(z + y + x);
	//direction = normalized(Vector3(pt, 0.0f) - camera->getFrom());
	scalar = 0.0f;
}

Ray::Ray(Vector3 orig, Vector3 dir)
{
	point = Vector3(0.0f);
	origin = orig;
	direction = normalized(dir);
	scalar = 0.0f;
}

Ray::~Ray(){}


