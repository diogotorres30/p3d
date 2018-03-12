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

void Camera::setAngle(float ang)
{
	angle = ang;
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
