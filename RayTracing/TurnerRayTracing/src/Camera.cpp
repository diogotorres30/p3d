#include "Camera.h"

Camera::Camera(Vector3 nFrom, Vector3 nAt, Vector3 nUp, float nAngle, float nHither, int nResX, int nResY)
{
	from = nFrom;
	at = nAt;
	up = nUp;
	angle = nAngle;
	hither = nHither;
	resX = nResX;
	resY = nResY;
}

Camera::~Camera()
{

}

//void Camera::setPosition(Vector3 pos)
//{
//	position = pos;
//}
//
//void Camera::setLookAt(Vector3 pos)
//{
//	lookAt = pos;
//}
//
//void Camera::setOrthographic(const float &left, const float &right, const float &bottom, const float &top, const float &zNear, const float &zFar)
//{
//	MatrixFactory mf;
//
//	orthographic = mf.orthographicMatrix(left, right, bottom, top, zNear, zFar);
//}
//
//void Camera::setPerspective(const float &nfovy, const float &naspect,const float &nzNear, const float &nzFar)
//{
//	MatrixFactory mf;
//
//	fovy = nfovy;
//	aspect = naspect;
//	zNear = nzNear;
//	zFar = nzFar;
//
//	perspective = mf.perspectiveMatrix(fovy, aspect, zNear, zFar);
//}
