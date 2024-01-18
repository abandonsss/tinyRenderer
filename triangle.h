#pragma once

#include "geometry.h"

class Triangle {
private:

public:
    Vec4f v[3];
    Vec2f texCoords[3];
    Vec3f normal[3];
    Vec3f color[3];

    Triangle();
    void computeFcolor(Vec3f light_dir);
    void computeGcolor(Vec3f light_dir);
};