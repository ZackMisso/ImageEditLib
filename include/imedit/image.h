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
#include "pixel.h"

#pragma once

namespace imedit
{

    // typedef double Float;
    typedef float Float;

    // clamps val between min and max
    static inline Float im_clamp(Float val, Float min, Float max)
    {
        return (val >= min) ? ((val <= max) ? val : max) : min;
    }

    // converts the value to a byte
    // static unsigned char valToByte(Float val)
    // {
    //     Float min = 0;
    //     Float max = 1;
    //     return int(255.0 * im_clamp(val, min, max));
    // }

    inline Float GammaCorrect(Float value)
    {
        if (value <= 0.0031308f)
            return 12.92f * value;
        return 1.055f * std::pow(value, (Float)(1.f / 2.4f)) - 0.055f;
    }

    // taken from PBRTv3
    static unsigned char valToByte(Float val)
    {
        Float min = 0.0;
        Float max = 255.0;
        return int(im_clamp((Float)255.0 * GammaCorrect(val) + (Float)0.5, min, max));
    }

    // converts the byte to a value
    // static void byteToVal(const unsigned char in, Float &val)
    // {
    //     val = ((Float)in) / ((Float)255.0);
    // }

    static void byteToVal(const unsigned char in, Float &val)
    {
        val = pow(((Float)in) / ((Float)255.0), 2.2);
    }

    // returns the extension of a file path
    static std::string getExtension(const std::string &filename)
    {
        if (filename.find_last_of(".") != std::string::npos)
            return filename.substr(filename.find_last_of(".") + 1);
        return "";
    }

    class RGBImage
    {
    public:
        // TODO: remove depth parameter, it is never going to be used
        RGBImage();
        RGBImage(int w, int h);
        RGBImage(const std::string &filename);

        void clear();

        Float average() const;
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
        bool read(const std::string &filename);
        bool write(const std::string &filename);
        // bool writeChannel(const std::string& filename, int ch);

        void resize(int cols, int rows);
        void setZero();

        // Image getChannel(int ch);

        float safeAccess(int j, int i, int k);

        void setPixels(Float r, Float b, Float g);
        void setPixels(Pixel p);
        void setPixel(int j, int i, Pixel p);

        void operator~();

        RGBImage operator+(const RGBImage &other) const;
        RGBImage operator-(const RGBImage &other) const;
        // this is component-wise multiplication
        RGBImage operator*(const RGBImage &other) const;
        // this is component-wise division
        RGBImage operator/(const RGBImage &other) const;
        RGBImage operator+(Float val) const;
        RGBImage operator-(Float val) const;
        RGBImage operator*(Float val) const;
        RGBImage operator/(Float val) const;
        void operator+=(const RGBImage &other);
        void operator-=(const RGBImage &other);
        // component wise multiplication
        void operator*=(const RGBImage &other);
        // component wise division
        void operator/=(const RGBImage &other);
        void operator+=(Float val);
        void operator-=(Float val);
        // component wise multiplication
        void operator*=(Float val);
        // component wise division
        void operator/=(Float val);

        Float &operator[](int index);
        Float operator[](int index) const;

        // Pixel& operator[](int index);
        // Pixel operator[](int index) const;

        Float &operator()(int x, int y, int z);
        Float operator()(int x, int y, int z) const;
        Float operator()(Float x, Float y, int z) const;
        Float &operator()(Float x, Float y, int z);

        Pixel &operator()(int x, int y);
        Pixel operator()(int x, int y) const;
        Pixel &operator()(Float x, Float y);
        Pixel operator()(Float x, Float y) const;

        Float &filter_index(int x, int y, int z);
        Float filter_index(int x, int y, int z) const;

        Pixel filter_index(int x, int y) const;

        int width() const { return w; }
        int height() const { return h; }
        int depth() const { return 3; }
        uint32_t size() const { return w * h * 3; }

    protected:
        std::vector<Pixel> pixels;

        int w, h;
    };

// this is a hack, will have to fix later
#define Image RGBImage

}
