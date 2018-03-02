#include "Camera.h"


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
