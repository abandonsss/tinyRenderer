#pragma once

#include "geometry.h"

class Camera {
public:
    Vec3f position;
    Vec3f worldUP;
    Vec3f front;
    Vec3f up;
    Vec3f right;

    Camera(Vec3f position_ = Vec3f(0.0f, 0.0f, 0.0f), Vec3f worldup_ = Vec3f(0.f, 1.f, 0.f), Vec3f front_ = Vec3f(0.f, 0.f, -1.f)) {
        position = position_;
        worldUP = worldup_;
        front = front_.normalize();
        right = (front ^ worldUP).normalize();
        up = (right ^ front).normalize();
    }
};