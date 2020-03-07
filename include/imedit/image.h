/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <string>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>

#pragma once

// useful functions
// TODO: rewrite all of this in terms of templates

namespace imedit
{

// typedef double Float;
typedef float Float;

struct Pixel
{
    Pixel() : r(0.f), g(0.f), b(0.f) { }
    Pixel(Float r, Float g, Float b) : r(r), g(g), b(b) { }

    Float r;
    Float g;
    Float b;
};

// clamps val between min and max
static inline Float im_clamp(Float val, Float min, Float max)
{
    return (val >= min) ?
           ((val <= max) ? val : max) : min;
}

// converts the value to a byte
static unsigned char valToByte(Float val)
{
    Float min = 0;
    Float max = 1;
    return int(255.0 * im_clamp(val, min, max));
}

// converts the byte to a value
static void byteToVal(const unsigned char in, Float& val)
{
    val = ((Float)in) / ((Float)255.0);
}

// returns the extension of a file path
static std::string getExtension(const std::string &filename)
{
	if (filename.find_last_of(".") != std::string::npos)
		return filename.substr(filename.find_last_of(".") + 1);
	return "";
}

enum ImageMode
{
    IM_GREYSCALE,
    IM_COLOR,
    IM_3D_COLORED_TEXTURE,
    IM_3D_GRAYSCALE_TEXTURE
};

class Image
{
public:
    Image();
    Image(int w, int h, int d);
    Image(int w, int h, int d, ImageMode mode);
    Image(const std::string& filename);

    void scale_to_fit(const Image& other);

    void clear();

    // TODO: convert this form of averaging to be in-place
    double average() const;

    // TODO: convert this form of averaging to be in-place
    Pixel average_pixel() const;

    Float max() const;
    Float min() const;

    Pixel max_channel() const;
    Pixel min_channel() const;

    void brighten(Float factor);
    void contrast(Float factor, Float midpoint);
    void exposure(Float factor);
    void alterGamma(Float oldGamma, Float newGamma);

    // The read and write logic is based off of code written by Wojciech Jarosz
    bool read(const std::string& filename);
    bool write(const std::string& filename);
    bool writeChannel(const std::string& filename, int ch);

    void resize(int cols, int rows, int channels);
    void setZero();

    Image getChannel(int ch);

    void setPixels(Float r, Float b, Float g);
    void setPixels(Pixel p);
    void setPixel(int j, int i, Pixel p);

    void operator~();

    Image operator+(const Image& other) const;
    Image operator-(const Image& other) const;
    // this is component-wise multiplication
    Image operator*(const Image& other) const;
    // this is component-wise division
    Image operator/(const Image& other) const;
    Image operator+(Float val) const;
    Image operator-(Float val) const;
    Image operator*(Float val) const;
    Image operator/(Float val) const;
    void operator+=(const Image& other);
    void operator-=(const Image& other);
    // component wise multiplication
    void operator*=(const Image& other);
    // component wise division
    void operator/=(const Image& other);
    void operator+=(Float val);
    void operator-=(Float val);
    // component wise multiplication
    void operator*=(Float val);
    // component wise division
    void operator/=(Float val);
    Float& operator[](int index);
    Float operator[](int index) const;
    Float& operator()(int x, int y, int z);
    Float operator()(int x, int y, int z) const;
    Float operator()(float x, float y, int z) const;
    Float& operator()(float x, float y, int z);

    Float& filter_index(int x, int y, int z);
    Float filter_index(int x, int y, int z) const;

    int width() const { return w; }
    int height() const { return h; }
    int depth() const { return d; }
    uint32_t size() const { return w * h * d; }

protected:
    std::vector<Float> im;

    ImageMode mode;
    int w, h, d;
};

}
