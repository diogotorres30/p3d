#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include <string>
#include <sstream>
#include "MathAux.h"
#include "Color.h"

class Material {

private:

    Color color;
    float kd;
    float ks;
    float shine;
    float t;
    float index_of_refraction;

public:
	Material() {};
    Material(Color c, float newkd, float newks, float newshine, float newt, float index);
    float getRed();
    float getGreen();
    float getBlue();
    float getKd();
    float getKs();
    float getShine();
    float getT();
    float getIndex();
    

};

#endif // !__MATERIAL_H__
