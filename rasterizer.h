#pragma once

#ifndef RASTERTIZER_H
#define RASTERTIZER_H

#include <functional> 
#include <optional>
#include "geometry.h"
#include "texture.h"
#include "model.h"
#include "shader.h"


namespace rst {
    enum class Buffers
	{
		Color = 1,
		Depth = 2
	};

    enum class Primitive
	{
		Line,
		Triangle
	};
    class rasterizer {
    private:
        Mat4f modelMatrix;
        Mat4f viewMatrix;
        Mat4f projectionMatrix;

        std::vector<float> depth_buffer;
        int width, height;
        std::optional<Texture> texture;
        std::function<Vec3f(fragment_shader_payload)> fragmentShader;
        std::function<Vec3f(vertex_shader_payload)> vertexShader;

        void draw_line();
        void draw_triangle(Triangle& t);
    public:
        rasterizer(int w, int h) : width(w), height(h) {
            depth_buffer.resize(w*h);
            frame_buffer.resize(w*h);
            texture = std::nullopt;
        }

        void set_model(const Mat4f& m);
        void set_view(const Mat4f& m);
        void set_projection(const Mat4f& m);
        void set_texture(Texture tex);
        void set_pixel(Vec2i& point, Vec3f& color);
        void set_vertexShader(std::function<Vec3f(vertex_shader_payload)>);
        void set_fragmentShader(std::function<Vec3f(fragment_shader_payload)>);

        void draw(std::vector<Triangle>& triangleList);
        std::vector<Vec3f> frame_buffer;
        void clear(Buffers buf);
    };
}
#endif