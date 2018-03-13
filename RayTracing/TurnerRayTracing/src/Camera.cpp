#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::setFrom(Vector3 f)
{
	from = f;
}

void Camera::setAt(Vector3 a)
{
	at = a;
}

void Camera::setUp(Vector3 u)
{
	up = u;
}

void Camera::setFov(float ang)
{
	fov = degreesToRadians(ang);
}

void Camera::setHither(float hit)
{
	hither = hit;
}

void Camera::setResolution(float x, float y)
{
	resX = x;
	resY = y;
}

void Camera::calculate()
{
	df = norm(from - at);

	h = 2.0f * df * tan(fov / 2);
	w = (resX / resY) * h;

	ze = (1 / df) * (from - at);
	Vector3 external = cross(up, ze);
	xe = (1 / norm(external)) * external;
	ye = cross(ze, xe);
}