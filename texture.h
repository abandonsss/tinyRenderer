#pragma once

#include "tgaimage.h"

class Texture {
private:
    TGAImage image_data;
public:
    Texture(const char* filename) {
        image_data.read_tga_file(filename);
        image_data.flip_vertically();

        width = image_data.width();
        height = image_data.height();
    }
    int width, height;

    TGAColor getColor(float u, float v) {
        
        return image_data.get(u, v);
    }
};