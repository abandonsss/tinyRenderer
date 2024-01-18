#pragma once

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include <cmath>
#include <ostream>

template <class t>
class Vec2 {
public:
    union  {
        struct {
            t x, y;
            t raw[2];
        };
    };
    Vec2<t>() : x(0), y(0) {}
    Vec2<t>(t _x, t _y) : x(_x), y(_y) {}
    Vec2<t>(const Vec2<t>& v) { *this = v; }
    Vec2<t>& operator =(const Vec2<t>& v) {
        if(this != &v) {
            this->x = v.x;
            this->y = v.y;
        }
        return *this;
    }
    inline Vec2<t> operator +(const Vec2<t> &v) const
    { return Vec2<t>(x+v.x, y+v.y); }
    inline Vec2<t> operator -(const Vec2<t> &v) const
    { return Vec2<t>(x-v.x, y-v.y); }
    inline Vec2<t> operator *(float f) const 
    { return Vec2<t>(x*f, y*f); }
    inline t& operator [](const int i)
    { if(i <= 0) return x; else return y; }
    float norm() const { return std::sqrt(x*x+y*y); }
    Vec2<t>& normalize( t l=1) { 
        *this = (*this) * (l / norm()); 
        return *this;
    }
    template <class> friend std::ostream& operator <<(std::ostream& o, Vec2<t> &v);
};

template <class t> 
std::ostream& operator <<(std::ostream& o, Vec2<t> &v) {
    o << "(" << v.x << "," << v.y << ")\n";
    return o;
}

typedef Vec2<int> Vec2i;
typedef Vec2<float> Vec2f;

template <class t> 
class Vec3 {
public:
    union {
        struct { t x,y,z; };
        t raw[3];
    };
    
    Vec3<t>() : x(0), y(0), z(0) {}
    Vec3<t>(t _x, t _y, t _z) : x(_x), y(_y), z(_z) {}
    Vec3<t>(const Vec3<t>& v) { *this = v; };
    Vec3<t>& operator =(const Vec3<t>& v) {
        if( this != &v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
        }
        return *this;
    }
    inline Vec3<t> operator +(const Vec3<t> &v) const 
    { return Vec3<t>(x+v.x, y+v.y, z+v.z); }
    inline Vec3<t> operator -(const Vec3<t> &v) const 
    { return Vec3<t>(x-v.x, y-v.y, z-v.z); }
    inline Vec3<t> operator *(float f) const
    { return Vec3<t>(x*f, y*f, z*f); }
    inline t operator *(const Vec3<t> &v) const 
    { return (x*v.x + y*v.y + z*v.z); }
    inline t& operator [](const int i) 
    { if(i <= 0) return x; else if(i==1) return y; else return z; }
    inline Vec3<t> operator ^(const Vec3<t> &v) const 
    { return Vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
    float norm() const { return std::sqrt(x*x+y*y+z*z); }
    Vec3<t> & normalize(t l = 1) 
    { *this = (*this)*(l/norm()); return *this; }

    template <class > friend std::ostream & operator<<(std::ostream &o, Vec3<t> &v);
};

template <class t>
std::ostream & operator<<(std::ostream &o, Vec3<t> &v) {
    o << "(" << v.x << "," << v.y << "," << v.z << ")\n";
    return o;
}

typedef Vec3<float> Vec3f;
typedef Vec3<int> Vec3i;

template <class t>
class Vec4 {
public:
    union  {
       struct{ t x,y,z,w; };
        t raw[4];
    };
    Vec4<t>() : x(0), y(0), z(0), w(0) {}
    Vec4<t>(t _x, t _y, t _z, t _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vec4<t>(const Vec4<t>& v) { *this = v; }
    Vec4<t>& operator =(const Vec4<t>& v) {
        if(this != &v) {
            this->x = v.x;
            this->y = v.y;
            this->z = v.z;
            this->w = v.w;
        }
        return *this;
    }
    inline Vec4<t> operator +(const Vec4<t>& V) const { return Vec4<t>(x + V.x, y + V.y, z + V.z, w + V.w); }
	inline Vec4<t> operator -(const Vec4<t>& V) const { return Vec4<t>(x - V.x, y - V.y, z - V.z, w - V.w); }
	inline Vec4<t> operator *(float f)          const { return Vec4<t>(x * f, y * f, z * f, w * f); }
	inline t       operator *(const Vec4<t>& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }
    inline t& operator [](const int idx) { if (idx <= 0) return x; else if (idx == 1) return y; else if (idx == 2) return z; else return w; }

    float norm() const { return std::sqrt(x * x + y * y + z * z + w * w); }
    Vec4<t>& normalize(t l = 1) { *this = (*this) * (l / norm()); return *this; }
    template <class> friend std::ostream& operator <<(std::ostream& o, Vec4<t>& v);
};
template <class t>
std::ostream& operator <<(std::ostream &o, Vec4<t> &v) {
    o << "(" << v.x << "," << v.y << "," << v.z << "," << v.w << ")\n";
    return o; 
}

typedef Vec4<int> Vec4i;
typedef Vec4<float> Vec4f;

class Mat3f {
public:
    Vec3f rows[3];

    Mat3f();
    
    Vec3f& operator[](const int i) { return rows[i]; }
    Vec3f operator[](const int i) const { return rows[i]; }
    Mat3f operator*(Mat3f& a);
    Vec3f operator*(Vec3f& a);

    Mat3f transpose();
    Mat3f inverse();

    static Mat3f identity();
    friend std::ostream& operator<<(std::ostream& o, Mat3f& a);
};

class Mat4f {
public:
    Vec4f rows[4];

    Mat4f();
    Vec4f& operator[](const int i) { return rows[i]; }
    Vec4f operator[](const int i) const { return rows[i]; }
    Mat4f operator*(Mat4f& a);
    Vec4f operator*(Vec4f& a);

    Mat4f transpose();
	Mat4f inverse();

    static Mat4f identity();
    friend std::ostream& operator<<(std::ostream& s, Mat4f& m);
};
#endif