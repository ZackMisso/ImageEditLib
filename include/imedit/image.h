/*
    ImageEditLib was developed by Zack Misso
    <zack441@mac.com>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <string>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include "pixel.h"

// defining this because my autoformmatter is dumb and
// "std::vector<Pixel<T>>"" doesnt compile because of ">>"
#define VEC(v) std::vector<v>

#pragma once

// TODO: create a color spectrum image class

namespace imedit
{

    // clamps val between min and max
    template <typename T>
    static inline T im_clamp(T val, T min, T max)
    {
        return (val >= min) ? ((val <= max) ? val : max) : min;
    }

    template <typename T>
    inline T gamma_correct(T value)
    {
        if (value <= 0.0031308f)
            return 12.92f * value;
        return 1.055f * std::pow(value, (T)(1.f / 2.4f)) - 0.055f;
    }

    // taken from PBRTv3
    template <typename T>
    static unsigned char valToByte(T val)
    {
        T min = (T)0.0;
        T max = (T)255.0;
        return int(im_clamp((T)255.0 * gamma_correct(val) + (T)0.5, min, max));
    }

    template <typename T>
    static void byteToVal(const unsigned char in, T &val)
    {
        T byt = (((T)in) - (T)0.5) / ((T)255.0);
        byt = (byt + 0.055) / (1.055);
        val = pow(byt, (T)2.4);
    }

    // returns the extension of a file path
    static std::string getExtension(const std::string &filename)
    {
        if (filename.find_last_of(".") != std::string::npos)
            return filename.substr(filename.find_last_of(".") + 1);
        return "";
    }

    template <typename T>
    class RGBImage
    {
    public:
        RGBImage();
        RGBImage(int w, int h);
        RGBImage(const std::string &filename);

        void clear();

        T average() const;
        Pixel<T> average_pixel() const;

        T max() const;
        T min() const;

        Pixel<T> max_channel() const;
        Pixel<T> min_channel() const;

        void brighten(T factor);
        void contrast(T factor, T midpoint);
        void exposure(T factor);
        void alterGamma(T oldGamma, T newGamma);

        void resize(int cols, int rows);
        void setZero();

        T safeAccess(int j, int i, int k);

        void setPixels(T r, T b, T g);
        void setPixels(Pixel<T> p);
        void setPixel(int j, int i, Pixel<T> p);

        void operator~();

        // TODO: speed test these operations, this seems slow
        RGBImage<T> operator+(const RGBImage<T> &other) const;
        RGBImage<T> operator-(const RGBImage<T> &other) const;
        RGBImage<T> operator*(const RGBImage<T> &other) const;
        RGBImage<T> operator/(const RGBImage<T> &other) const;
        RGBImage<T> operator+(T val) const;
        RGBImage<T> operator-(T val) const;
        RGBImage<T> operator*(T val) const;
        RGBImage<T> operator/(T val) const;
        void operator+=(const RGBImage<T> &other);
        void operator-=(const RGBImage<T> &other);
        void operator*=(const RGBImage<T> &other);
        void operator/=(const RGBImage<T> &other);
        void operator+=(T val);
        void operator-=(T val);
        void operator*=(T val);
        void operator/=(T val);

        T &operator[](int index);
        T operator[](int index) const;

        // Pixel& operator[](int index);
        // Pixel operator[](int index) const;

        T &operator()(int x, int y, int z);
        T operator()(int x, int y, int z) const;
        T operator()(T x, T y, int z) const;
        T &operator()(T x, T y, int z);

        Pixel<T> &operator()(int x, int y);
        Pixel<T> operator()(int x, int y) const;
        Pixel<T> &operator()(T x, T y);
        Pixel<T> operator()(T x, T y) const;

        T &filter_index(int x, int y, int z);
        T filter_index(int x, int y, int z) const;

        Pixel<T> filter_index(int x, int y) const;

        int width() const { return w; }
        int height() const { return h; }
        int depth() const { return 3; }
        uint32_t size() const { return w * h * 3; }

    protected:
        VEC(Pixel<T>)
        pixels;

        int w, h;
    };

    template <typename T>
    RGBImage<T>::RGBImage() : w(0), h(0)
    {
        pixels = VEC(Pixel<T>)();
    }

    template <typename T>
    RGBImage<T>::RGBImage(int w, int h) : w(w), h(h)
    {
        pixels = VEC(Pixel<T>)(w * h);
    }

    template <typename T>
    RGBImage<T>::RGBImage(const std::string &filename)
    {
        pixels = VEC(Pixel<T>)();
        w = 0;
        h = 0;

        image_read(filename, *this);
    }

    template <typename T>
    void RGBImage<T>::clear()
    {
        int siz = w * h;
        for (int i = 0; i < siz; ++i)
        {
            pixels[i] = Pixel<T>(0);
        }
    }

    template <typename T>
    T RGBImage<T>::average() const
    {
        int siz = w * h;

        T avg = 0.f;

        for (int i = 0; i < siz; ++i)
        {
            avg += pixels[i].sum();
        }

        avg /= T(siz * 3);

        return avg;
    }

    template <typename T>
    Pixel<T> RGBImage<T>::average_pixel() const
    {
        Pixel<T> avg = Pixel<T>();
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            avg += pixels[i];
        }

        return avg / T(siz);
    }

    template <typename T>
    T RGBImage<T>::max() const
    {
        T val = pixels[0].r;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            val = std::max(val, pixels[i].max());
        }

        return val;
    }

    template <typename T>
    T RGBImage<T>::min() const
    {
        T val = pixels[0].r;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            val = std::min(val, pixels[i].min());
        }

        return val;
    }

    template <typename T>
    Pixel<T> RGBImage<T>::max_channel() const
    {
        Pixel<T> max = pixels[0];
        int siz = h * w;

        for (int i = 1; i < siz; ++i)
        {
            max = Pixel<T>::max(max, pixels[i]);
        }

        return max;
    }

    template <typename T>
    Pixel<T> RGBImage<T>::min_channel() const
    {
        Pixel<T> min = pixels[0];
        int siz = h * w;

        for (int i = 1; i < siz; ++i)
        {
            min = Pixel<T>::min(min, pixels[i]);
        }

        return min;
    }

    template <typename T>
    void RGBImage<T>::brighten(T factor)
    {
        (operator*=)(factor);
    }

    template <typename T>
    void RGBImage<T>::contrast(T factor, T midpoint)
    {
        (operator-=)(midpoint);
        (operator*=)(factor);
        (operator+=)(midpoint);
    }

    template <typename T>
    void RGBImage<T>::exposure(T factor)
    {
        alterGamma(T(1.0 / 2.2), T(1.0));
        brighten(factor);
        alterGamma(T(1.0), T(1.0 / 2.2));
    }

    template <typename T>
    void RGBImage<T>::alterGamma(T oldGamma, T newGamma)
    {
        T power = newGamma / oldGamma;
        int siz = h * w;

        for (int i = 0; i < siz; ++i)
        {
            pixels[i].r = pow(pixels[i].r, power);
            pixels[i].g = pow(pixels[i].g, power);
            pixels[i].b = pow(pixels[i].b, power);
        }
    }

    // bool RGBImage::writeChannel(const std::string& filename, int ch)
    // {
    //     return getChannel(ch).write(filename);
    // }

    template <typename T>
    T RGBImage<T>::safeAccess(int j, int i, int k)
    {
        if (j < 0)
            j = 0;
        if (j >= w)
            j = w - 1;
        if (i < 0)
            i = 0;
        if (i >= h)
            i = h;
        if (k < 0)
            k = 0;
        if (k >= 3)
            k = 2;

        return operator()(j, i, k);
    }

    template <typename T>
    void RGBImage<T>::resize(int cols, int rows)
    {
        pixels.resize(cols * rows);
        // TODO - not completely correct
        w = cols;
        h = rows;
    }

    template <typename T>
    void RGBImage<T>::setZero()
    {
        for (int i = 0; i < pixels.size(); ++i)
        {
            pixels[i] = Pixel<T>();
        }
    }

    // Image RGBImage::getChannel(int ch)
    // {
    //     Image image = Image(w, h);
    //
    //     for (int i = 0; i < h; ++i)
    //     {
    //         for (int j = 0; j < w; ++j)
    //         {
    //             image(j, h) = operator()(j, h);
    //         }
    //     }
    //
    //     return image;
    // }

    template <typename T>
    void RGBImage<T>::setPixels(T r, T g, T b)
    {
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                operator()(j, i, 0) = r;
                operator()(j, i, 1) = g;
                operator()(j, i, 2) = b;
            }
        }
    }

    template <typename T>
    void RGBImage<T>::setPixels(Pixel<T> p)
    {
        setPixels(p.r, p.g, p.b);
    }

    template <typename T>
    void RGBImage<T>::setPixel(int j, int i, Pixel<T> p)
    {
        operator()(j, i, 0) = p.r;
        operator()(j, i, 1) = p.g;
        operator()(j, i, 2) = p.b;
    }

    template <typename T>
    void RGBImage<T>::operator~()
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) = 1.f - operator[](i);
        }
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator+(const RGBImage<T> &other) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + other[i];
        }

        return image;
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator-(const RGBImage<T> &other) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - other[i];
        }

        return image;
    }

    // this is component-wise multiplication
    template <typename T>
    RGBImage<T> RGBImage<T>::operator*(const RGBImage<T> &other) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * other[i];
        }

        return image;
    }

    // this is component-wise division
    template <typename T>
    RGBImage<T> RGBImage<T>::operator/(const RGBImage<T> &other) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / other[i];
        }

        return image;
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator+(T val) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + val;
        }

        return image;
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator-(T val) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - val;
        }

        return image;
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator*(T val) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * val;
        }

        return image;
    }

    template <typename T>
    RGBImage<T> RGBImage<T>::operator/(T val) const
    {
        RGBImage<T> image = RGBImage<T>(w, h);

        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / val;
        }

        return image;
    }

    template <typename T>
    void RGBImage<T>::operator+=(const RGBImage<T> &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += other[i];
        }
    }

    template <typename T>
    void RGBImage<T>::operator-=(const RGBImage<T> &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= other[i];
        }
    }

    // component wise multiplication
    template <typename T>
    void RGBImage<T>::operator*=(const RGBImage<T> &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= other[i];
        }
    }

    // component wise division
    template <typename T>
    void RGBImage<T>::operator/=(const RGBImage<T> &other)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= other[i];
        }
    }

    template <typename T>
    void RGBImage<T>::operator+=(T val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += val;
        }
    }

    template <typename T>
    void RGBImage<T>::operator-=(T val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= val;
        }
    }

    // component wise multiplication
    template <typename T>
    void RGBImage<T>::operator*=(T val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= val;
        }
    }

    // component wise division
    template <typename T>
    void RGBImage<T>::operator/=(T val)
    {
        int size = w * h * 3;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= val;
        }
    }

    template <typename T>
    T &RGBImage<T>::operator[](int index)
    {
        int loc = index / 3;

        return pixels[loc].access(index);
    }

    template <typename T>
    T RGBImage<T>::operator[](int index) const
    {
        int loc = index / 3;

        return pixels[loc].const_access(index);
    }

    template <typename T>
    T &RGBImage<T>::operator()(int x, int y, int z)
    {
        int index = w * y + x;
        return pixels[index].access(z);
    }

    template <typename T>
    T RGBImage<T>::operator()(int x, int y, int z) const
    {
        int index = w * y + x;
        return pixels[index].const_access(z);
    }

    template <typename T>
    T RGBImage<T>::operator()(T x, T y, int z) const
    {
        // TODO
        return operator()(int(x), int(y), z);
    }

    template <typename T>
    T &RGBImage<T>::operator()(T x, T y, int z)
    {
        // TODO
        return operator()(int(x), int(y), z);
    }

    template <typename T>
    Pixel<T> &RGBImage<T>::operator()(int x, int y)
    {
        return pixels[y * w + x];
    }

    template <typename T>
    Pixel<T> RGBImage<T>::operator()(int x, int y) const
    {
        return pixels[y * w + x];
    }

    template <typename T>
    Pixel<T> &RGBImage<T>::operator()(T x, T y)
    {
        // TODO
        return pixels[0];
    }

    template <typename T>
    Pixel<T> RGBImage<T>::operator()(T x, T y) const
    {
        // TODO
        return Pixel<T>();
    }

    template <typename T>
    T &RGBImage<T>::filter_index(int x, int y, int z)
    {
        if (x < 0)
            x = 0;
        if (x > w - 1)
            x = w - 1;
        if (y < 0)
            y = 0;
        if (y > h - 1)
            y = h - 1;
        if (z < 0)
            z = 0;
        if (z > 2)
            z = 2;

        return pixels[y * w + x].access(z);
    }

    template <typename T>
    T RGBImage<T>::filter_index(int x, int y, int z) const
    {
        if (x < 0)
            x = 0;
        if (x > w - 1)
            x = w - 1;
        if (y < 0)
            y = 0;
        if (y > h - 1)
            y = h - 1;
        if (z < 0)
            z = 0;
        if (z > 2)
            z = 2;

        return pixels[y * w + x].const_access(z);
    }

    template <typename T>
    Pixel<T> RGBImage<T>::filter_index(int x, int y) const
    {
        if (x < 0)
            x = 0;
        if (x > w - 1)
            x = w - 1;
        if (y < 0)
            y = 0;
        if (y > h - 1)
            y = h - 1;

        return pixels[y * w + x];
    }

    bool image_read(const std::string &filename, RGBImage<float> &image);
    bool image_write(const std::string &filename, RGBImage<float> &image);

    bool image_read(const std::string &filename, RGBImage<double> &image);
    bool image_write(const std::string &filename, RGBImage<double> &image);
}
