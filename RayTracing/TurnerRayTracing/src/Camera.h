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
		Camera();
		~Camera();
		void setFrom(Vector3 f);
		void setAt(Vector3 a);
		void setUp(Vector3 u);
		void setAngle(float ang);
		void setHither(float hit);
		void setResolution(float x, float y);
		Vector3 getFrom() { return from; };
		int getResX() { return resX; };
		int getResY() { return resY; };
};

#endif // !__CAMERA_H__