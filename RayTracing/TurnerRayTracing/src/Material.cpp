#include "Material.h"

Material::Material(Color c, float newkd, float newks, float newshine, float newt, float index)
{
    color = c;
    kd = newkd;
    ks = newks;
    shine = newshine;
    t = newt;
    index_of_refraction = index;
}


/***************** GETTERS ******************/

Color Material::getColor()
{
	return color;
}

float Material::getKd() {
    return kd;
}

float Material::getKs() {
    return ks;
}

float Material::getShine() {
    return shine;
}

float Material::getT() {
    return t;
}

float Material::getIndex() {
    return index_of_refraction;
}

float Material::getRed() {
	return color.r;
}

float Material::getGreen() {
	return color.g;
}

float Material::getBlue() {
	return color.b;
}