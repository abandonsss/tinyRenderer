#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#pragma pack(push,1)
struct TGAHeader
{ 
    std::uint8_t  idlength = 0;
    std::uint8_t  colormaptype = 0;
    std::uint8_t  datatypecode = 0;
    std::uint16_t colormaporigin = 0;
    std::uint16_t colormaplength = 0;
    std::uint8_t  colormapdepth = 0;
    std::uint16_t x_origin = 0;
    std::uint16_t y_origin = 0;
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::uint8_t  bitsperpixel = 0;
    std::uint8_t  imagedescriptor = 0;
};
#pragma pack(pop)

struct TGAColor
{
    TGAColor() {};
    TGAColor(int b,int g,int r, int a = 255) { 
        bgra[0] = b; bgra[1] = g; bgra[2] = r; bgra[3] = a;
    }
    TGAColor(const TGAColor &c) : bytespp(c.bytespp) {
         bgra[0] = c.bgra[0]; bgra[1] = c.bgra[1];
         bgra[2] = c.bgra[2]; bgra[3] = c.bgra[3];
    }
    union   {
       std::uint8_t bgra[4] = {0,0,0,0};
       struct {
            std::uint8_t b,g,r,a;
       };
       
    };
    std::uint8_t bytespp = 4;
    TGAColor operator *(float intensity) {
        TGAColor res = *this;
        intensity = (intensity > 1.f ? 1.f : (intensity < 0.f ? 0.f : intensity));
        for(int i = 0; i < 4; i++) 
            res.bgra[i] = res.bgra[i] * intensity;
        return res;
    }
    std::uint8_t& operator[](const int i) { return bgra[i]; }
};

class TGAImage 
{
public:
    enum Format { GRAYSCALE=1, RGB=3, RGBA=4 };

    TGAImage() = default;
    TGAImage(const int w, const int h, const int bpp);
    bool read_tga_file(const std::string filename);
    bool write_tga_file(const std::string filename, const bool vflip =true, const bool rle = true) const;
    void flip_horizontally();
    void flip_vertically();
    TGAColor get(const int x, const int y) const;
    void set(const int x, const int y, const TGAColor &c);
    int width() const;
    int height() const;
private:
    bool load_rle_data(std::ifstream &in);
    bool unload_rle_data(std::ofstream &out) const;

    int w = 0;
    int h = 0;
    std::uint8_t bpp = 0;
    std::vector<std::uint8_t> data = {};
};
