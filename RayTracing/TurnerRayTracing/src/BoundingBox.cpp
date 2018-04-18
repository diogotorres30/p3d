#include "BoundingBox.h"

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(float minX, float maxX, float minY, float maxY, float minZ, float maxZ){
    xMax = maxX;
    yMax = maxY;
    zMax = maxZ;
    xMin = minX;
    yMin = minY;
    zMin = minZ;
    
}

float BoundingBox::intersect(Ray &ray){
    float t = -1.0f;
    float a = 1.0f / ray.direction.x;
    float tx_min, tx_max;
    
    if(a >= 0.0f){
        tx_min = (xMin - ray.origin.x) * a;
        tx_max = (xMax - ray.origin.x) * a;
    }
    else{
        tx_min = (xMax - ray.origin.x) * a;
        tx_max = (xMin - ray.origin.x) * a;
    }
    
    float b = 1.0f / ray.direction.y;
    float ty_min, ty_max;
    
    if(b >= 0.0f){
        ty_min = (yMin - ray.origin.y) * b;
        ty_max = (yMax - ray.origin.y) * b;
    }
    else{
        ty_min = (yMax - ray.origin.y) * b;
        ty_max = (yMin - ray.origin.y) * b;
    }
    
    float c = 1.0f / ray.direction.z;
    float tz_min, tz_max;
    
    if(c >= 0.0f){
        tz_min = (zMin - ray.origin.z) * c;
        tz_max = (zMax - ray.origin.z) * c;
    }
    else{
        tz_min = (zMax - ray.origin.z) * c;
        tz_max = (zMin - ray.origin.z) * c;
    }
    
    float t0, t1;
    
    if(tx_min > ty_min){
        t0 = tx_min;
    }
    else{
        t0 = ty_min;
    }
    
    if(tz_min > t0){
        t1 = tz_min;
    }
    
    if(tx_max > ty_max){
        t1 = tx_max;
    }
    else{
        t1 = ty_max;
    }
    
    if(tz_max > t0){
        t1 = tz_max;
    }
    
    if(t0 < t1 && t1 > 0.0001f){
        // ou e t0 ou t1 ??????
        t = t0;
    }
    
    return t;
}

Vector3 BoundingBox::getNormal(Ray &ray){
    return normal;
}
