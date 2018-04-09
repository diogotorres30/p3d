#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathAux.h"

class Camera
{
private:
	Vector3 from, at, up;
	Vector3 xe, ye, ze;
	float fov, hither, h, w, df, zNear, zFar;
	int resX, resY;

public:
	Camera();
	~Camera();
	void setFrom(Vector3 f);
	void setAt(Vector3 a);
	void setUp(Vector3 u);
	void setFov(float ang);
	void setHither(float hit);
	void setResolution(int x, int y);
	Vector3 getFrom() { return from; };
	float getHither() { return hither; };
	int getResX() { return resX; };
	int getResY() { return resY; };
	Vector3 getXe() { return xe; };
	Vector3 getYe() { return ye; };
	Vector3 getZe() { return ze; };
	float getDf() { return df; };
	float getHeight() { return h; };
	float getWidth() { return w; };
	void calculate();
};

#endif // !__CAMERA_H__

