#include "Ray.h"

Ray::Ray(){}

Ray::Ray(Camera *camera, Vector2 pt)
{
	point = Vector3(0.0f);
	origin = camera->getFrom();
	direction = normalized(-camera->getDf()*camera->getZe() + camera->getHeight()*((camera->getFrom().y / camera->getResY()) - 0.5)*camera->getYe() + camera->getWidth()*((camera->getFrom().x / camera->getResY()) - 0.5)*camera->getXe());
	//direction = normalized(Vector3(pt, 0.0f) - camera->getFrom());
	scalar = 0;
}

Ray::~Ray(){}


