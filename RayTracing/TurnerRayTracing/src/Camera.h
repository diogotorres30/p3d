#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathAux.h"

class Camera
{
	private:
		Vector3 from, at, up;
		float angle, hither;
		int resX, resY;
		float fovy, aspect, zNear, zFar, speed;
		Matrix4 projection, orthographic, perspective;
		Matrix4 viewMatrix;

	public:
		Camera(Vector3 nFrom, Vector3 nAt, Vector3 nUp, float nAngle, float nHither, int nResX, int nResY);
		~Camera();
		Vector3 getFrom() { return from; };
		int getResX() { return resX; };
		int getResY() { return resY; };
};

#endif // !__CAMERA_H__