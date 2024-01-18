#include "triangle.h"

Triangle::Triangle() {
    v[0] = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	v[1] = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);
	v[2] = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

	color[0] = Vec3f(0.0f, 0.0f, 0.0f);
	color[1] = Vec3f(0.0f, 0.0f, 0.0f);
	color[2] = Vec3f(0.0f, 0.0f, 0.0f);

	texCoords[0] = Vec2f(0.0f, 0.0f);
	texCoords[1] = Vec2f(0.0f, 0.0f);

	normal[0] = Vec3f(0.0f, 0.0f, 0.0f);
	normal[1] = Vec3f(0.0f, 0.0f, 0.0f);
	normal[2] = Vec3f(0.0f, 0.0f, 0.0f);
}

void Triangle::computeFcolor(Vec3f light_dir) {
    Vec3f normal_ = (Vec3f(v[2].x, v[2].y, v[2].z) - Vec3f(v[0].x, v[0].y, v[0].z)) ^ (Vec3f(v[1].x, v[1].y, v[1].z) - Vec3f(v[0].x, v[0].y, v[0].z));
	normal_.normalize();
	float intensity = std::max(0.f, normal_ * light_dir.normalize());
	for(int i = 0; i < 3; i++) 
		color[i] = Vec3f(255, 255, 255) * intensity;
}

void Triangle::computeGcolor(Vec3f light_dir) {
	for(int i = 0; i < 3; i++) {
		float intensity = std::max(0.f, normal[i].normalize() * light_dir.normalize());
		color[i] = Vec3f(255, 255, 255) * intensity;
	}
}
