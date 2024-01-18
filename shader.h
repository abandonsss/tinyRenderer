#pragma once

#include "geometry.h"
#include "texture.h"

struct fragment_shader_payload  {   
    fragment_shader_payload() {
        texture = nullptr;
    }
    fragment_shader_payload(const Vec3f& color, const Vec3f& normal, const Vec2f& texCoord, Texture* tex) :
        color(color), normal(normal), tex_coords(texCoord), texture(tex) {}
    
    Vec3f view_pos;
    Vec3f color;
    Vec3f normal;
    Vec2f tex_coords;
    Texture* texture;
};

struct vertex_shader_payload {
    Vec3f position;
};

class Shader {
public:
    virtual Vec3f fragment_shader(fragment_shader_payload& payload) = 0;
    virtual Vec3f vertex_shader(vertex_shader_payload& payload) = 0;
};