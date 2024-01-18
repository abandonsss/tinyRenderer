#include "rasterizer.h"

extern Vec3f lightDir;

void rst::rasterizer::set_model(const Mat4f& m) {
    modelMatrix = m;
}

void rst::rasterizer::set_view(const Mat4f& v)
{
	viewMatrix = v;
}

void rst::rasterizer::set_projection(const Mat4f& p)
{
	projectionMatrix = p;
}

void rst::rasterizer::set_texture(Texture tex) {
    texture = tex;
}

void rst::rasterizer::clear(Buffers buf) {
    if(buf == Buffers::Color) 
        std::fill(frame_buffer.begin(), frame_buffer.end(), Vec3f(0.0, 0.0, 0.0));
    else if(buf == Buffers::Depth)
        std::fill(depth_buffer.begin(), depth_buffer.end(), -std::numeric_limits<float>::infinity());
}

void rst::rasterizer::set_pixel(Vec2i& point, Vec3f& color) {
    int index = point.x+point.y*width;
    frame_buffer[index] = color;
}

void rst::rasterizer::set_fragmentShader(std::function<Vec3f(fragment_shader_payload)> f) {
    fragmentShader = f;
}

void rst::rasterizer::set_vertexShader(std::function<Vec3f(vertex_shader_payload)> f) {
    vertexShader = f;
}

void rst::rasterizer::draw(std::vector<Triangle>& triangleList) {
    float f1 = (255 - 0.0) / 2.0;
    float f2 = (255 + 0.0) /2.0;
    
    Mat4f mvp = projectionMatrix * viewMatrix * modelMatrix;
    for(auto& t : triangleList) {
        int depth = 255;
        Triangle newtri = t;
        Vec4f v[3];
        for(int i=0;i<3;i++) { 
            v[i] = mvp * t.v[i];
        }

        for(auto& vec : v) {
            vec.x = vec.x / vec.w;
            vec.y = vec.y / vec.w;
            vec.z = vec.z / vec.w;
        }

        for (auto& vec : v)
        {
            float w = width * 3.f / 4.f;
            float h = height * 3.f / 4.f;
            float x = width / 8.f;
            float y = height / 8.f;

            vec.x = w / 2.f * vec.x + w / 2.f + x;
            vec.y = h / 2.f * vec.y + h / 2.f + y;
            vec.z = vec.z * f1 + f2;
        }

        for(int i=0; i<3; i++) {
            newtri.v[i]= v[i];
        }
        draw_triangle(newtri);
    }

}

static std::tuple<float, float, float> computeBarycentic2D(const Vec4f* pts, float x, float y) {
    float xa = pts[0].x;
    float ya = pts[0].y;
    float xb = pts[1].x;
    float yb = pts[1].y;
    float xc = pts[2].x;
    float yc = pts[2].y;

    float gamma = ((ya - yb) * x + (xb - xa) * y + xa * yb - xb * ya) / ((ya - yb) * xc + (xb - xa) * yc + xa * yb - xb * ya);
    float beta = ((ya - yc) * x + (xc - xa) * y + xa * yc - xc * ya) / ((ya - yc) * xb + (xc - xa) * yb + xa * yc - xc * ya);
    float alpha = 1 - gamma - beta;

    return { alpha, beta, gamma };
}

void rst::rasterizer::draw_triangle(Triangle& t) {
    const Vec4f* pts = t.v;
    
    float minx = std::min({t.v[0].x, t.v[1].x, t.v[2].x});
    float maxx = std::max({ t.v[0].x, t.v[1].x, t.v[2].x });
    float miny = std::min({ t.v[0].y, t.v[1].y, t.v[2].y });
    float maxy = std::max({ t.v[0].y, t.v[1].y, t.v[2].y });
    int min_x = (int)std::floor(minx);
    int max_x = (int)std::ceil(maxx);
    int min_y = (int)std::floor(miny);
    int max_y = (int)std::ceil(maxy);
    t.computeGcolor(lightDir);
    for(int i = min_x; i <= max_x; i++) {
        for(int j = min_y; j <= max_y; j++) {
            Vec2i point(i, j);
            auto[alpha, beta, gamma] = computeBarycentic2D(pts, static_cast<float>(i + 0.5), static_cast<float>(j + 0.5)); 
            if(alpha < -0.01 || beta < -0.01 || gamma < -0.01)
                continue;
            float z_interpolation = pts[0].z * alpha + beta * pts[1].z + gamma * pts[2].z;
            Vec2f uv_interpolation = t.texCoords[0] * alpha + t.texCoords[1] * beta+ t.texCoords[2] * gamma;
            Vec3f color_interpolation = t.color[0] * alpha + t.color[1] * beta + t.color[2] * gamma;
            Vec3f normal_interpolation = t.normal[0] * alpha + t.normal[1] * beta + t.normal[2] * gamma;

            fragment_shader_payload payload(color_interpolation, normal_interpolation, uv_interpolation, texture ? &*texture : nullptr);
            if(z_interpolation > depth_buffer[static_cast<int>(i + j * width)]) {
                depth_buffer[static_cast<int>(i + j * width)] = z_interpolation;
                auto pixel_color = fragmentShader(payload);
                set_pixel(point, pixel_color);
            }
        }
    }
}