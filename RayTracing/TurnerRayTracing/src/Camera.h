#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "MathAux.h"

class Camera
{
private:
    Vector3 from = NULL;
    Vector3 at = 0;
    Vector3 up = 0;
    Vector3 xe, ye, ze;
    float fov = 0.0f;
    float hither, h, w, df, zNear, zFar;
    int resX = 512;
    int resY = 512;
    float cate;
    
public:
    Camera();
    ~Camera();
    void setFrom(Vector3 f);
    void setAt(Vector3 a);
    void setUp(Vector3 u);
    void setFov(float ang);
    void setHither(float hit);
    void setResolution(float x, float y);
    int getResX() { return resX; };;
    int getResY() { return resY; };
    Vector3 getXe() { return xe; };
    Vector3 getYe() { return ye; };
    Vector3 getZe() { return ze; };
    Vector3 getFrom() { return from; };
    float getDf() { return df; };
    float getHeight() { return h; };
    float getWidth() { return w; };
    void calculate();
};

#endif // !__CAMERA_H__

