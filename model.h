#pragma once

#ifndef _MODEL_H__
#define _MODEL_H__

#include <vector>

#include "geometry.h"
#include "tgaimage.h"
#include "texture.h"
#include "triangle.h"

class Model {
private:
    Texture* tex = nullptr;
    int vertNum=0;
    int faceNum=0;
public:
    std::vector<Triangle> triangleList={};
    
    Model(const char* filename);
    ~Model();
    int nverts();
    int nfaces();
};

#endif