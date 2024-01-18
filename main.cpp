#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include "shader.h"
#include "rasterizer.h"
#include "camera.h"
#include <iostream>
#include <string>

const TGAColor white(255, 255, 255, 255);
const TGAColor green(0, 0, 255, 255);
const TGAColor red(0, 255, 0, 255);
const TGAColor blue(255, 0, 0, 255);
const int width = 800;
const int height = 800;
Model* model = NULL;

Vec3f center(0.f, 0.f, 0.f); // 相机面对位置
Vec3f cameraPos(1.f, 1.f, 3.f); // 摄像机位置
Vec3f lightDir(0.f, 0.f, 1.f); //光线方向
Vec3f cameraUp(0.f, 1.f, 0.f);
Camera camera(cameraPos, cameraUp, center - cameraPos);

Mat4f modelMatrix() {
    return Mat4f::identity();
}

Mat4f viewMatrix() {
    Mat4f ret;
    ret[0] = Vec4f(camera.right.x, camera.right.y, camera.right.z, -camera.position.x);
    ret[1] = Vec4f(camera.up.x, camera.up.y, camera.up.z, -camera.position.y);
    ret[2] = Vec4f(-camera.front.x, -camera.front.y, -camera.front.z, -camera.position.z);
    ret[3] = Vec4f(0.f, 0.f, 0.f, 1.f);
    return ret;
}

Mat4f projectionMatrix() {
    Mat4f projection = Mat4f::identity();
    projection[3][2] = -1.0f / cameraPos.z;
    return projection;
}

Vec3f barycentric(Vec3f* pts, Vec3f p) {
    Vec3f u = Vec3f(pts[1].x-pts[0].x, pts[2].x-pts[0].x, pts[0].x-p.x)^Vec3f(pts[1].y-pts[0].y, pts[2].y-pts[0].y, pts[0].y-p.y);
    if(std::abs(u.z) < 1) 
        return Vec3f(-1,1,1);
    return Vec3f(1-(u.x+u.y)/u.z, u.x/u.z, u.y/u.z);
}

Vec3f vertex_shader(const vertex_shader_payload& payload) {
    return payload.position;
}

Vec3f F_fragment_shader(const fragment_shader_payload& payload) {
    return payload.color;
}

Vec3f G_fragment_shader(const fragment_shader_payload& payload) {
    return payload.color;
}

Vec3f normal_fragment_shader(const fragment_shader_payload& payload) {
    Vec3f normal_frag = payload.normal;
    Vec3f color = (normal_frag.normalize() + Vec3f(1.0f, 1.0f, 1.0f)) * 0.5;
    return Vec3f(color.x * 255, color.y * 255, color.z*255);
}

int main(int argc, char** argv){
    if(2 == argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("../head.obj");
    }

    TGAImage image(width, height, TGAImage::Format::RGB);
    rst::rasterizer r(width, height);
    Texture tex("../diffuse.tga");
    
    r.clear(rst::Buffers::Color);
    r.clear(rst::Buffers::Depth);
    r.set_model(modelMatrix());
    r.set_view(viewMatrix());
    r.set_projection(projectionMatrix());
    r.set_texture(tex);
    r.set_vertexShader(vertex_shader);
    r.set_fragmentShader(G_fragment_shader);
    r.draw(model->triangleList);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            Vec3f color = r.frame_buffer[static_cast<int>(i+j*width)];
            image.set(i,j, TGAColor(color.x, color.y, color.z, 255));
        }
    }
    image.write_tga_file("output.tga");
    return 0;
}

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if(std::abs(x0 - x1) < std::abs(y0 - y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if(x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        if (steep) {
            image.set(y, x, color);
        }
        else {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

