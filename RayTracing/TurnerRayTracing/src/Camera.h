#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathAux.h"

class Camera
{
	private:
		Vector3 position, lookAt, direction, up;
		Vector3 invposition, invlookAt, invdirection, invup;
		float fovy, aspect, zNear, zFar, speed;
		Matrix4 projection, orthographic, perspective;
		Matrix4 viewMatrix, invviewMatrix;
		bool updatebool;
		int timeamount = 0;
		const int maxtime = 150;
		int RES_X, RES_Y;

	public:
		Camera();
		~Camera();
		int GetResX() { return RES_X; };
		int GetResY() { return RES_Y; };
};

#endif // !__CAMERA_H__