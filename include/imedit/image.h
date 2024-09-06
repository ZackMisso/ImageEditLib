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

// #define STB_IMAGE_STATIC
// #define STB_IMAGE_WRITE_STATIC

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb/stb_image.h>

// #define STB_IMAGE_WRITE_IMPLEMENTATION
// #include <stb/stb_image_write.h>

// #define TINYEXR_IMPLEMENTATION
// #include <tinyexr/tinyexr.h>

#pragma once

// TODO: create a color spectrum image class

namespace imedit
{
    // typedef double Float;
    // typedef float Float;

    // clamps val between min and max
    template <typename T>
    static inline T im_clamp(T val, T min, T max)
    {
        return (val >= min) ? ((val <= max) ? val : max) : min;
    }

    // converts the value to a byte
    // template <typename T>
    // static unsigned char valToByte(T val)
    // {
    //     T min = 0;
    //     T max = 1;
    //     return int(255.0 * im_clamp(val, min, max));
    // }

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

    // converts the byte to a value
    // static void byteToVal(const unsigned char in, Float &val)
    // {
    //     val = ((Float)in) / ((Float)255.0);
    // }

    // template <typename T>
    // static void byteToVal(const unsigned char in, T &val)
    // {
    //     // TODO
    //     val = pow(((T)in) / ((T)255.0), 2.2);
    // }

    template <typename T>
    static void byteToVal(const unsigned char in, T &val)
    {
        val = pow(((T)in) / ((T)255.0), 2.2);
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

        // The read and write logic is based off of code written by Wojciech Jarosz
        // bool read(const std::string &filename);
        // bool write(const std::string &filename);

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

// this is a hack, will have to fix later
#define Image RGBImage

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

    // template <typename T>
    // bool RGBImage<T>::read(const std::string &filename)
    // {
    //     int wid;
    //     int hei;
    //     int dep;

    //     try
    //     {
    //         if (getExtension(filename) == "txt")
    //         {
    //             std::string line;
    //             std::ifstream file(filename);

    //             std::getline(file, line);
    //             int wid = std::stoi(line);
    //             std::getline(file, line);
    //             int hei = std::stoi(line);

    //             resize(wid, hei);

    //             for (int y = 0; y < h; ++y)
    //             {
    //                 for (int x = 0; x < w; ++x)
    //                 {
    //                     for (int z = 0; z < 3; ++z)
    //                     {
    //                         std::getline(file, line);
    //                         operator()(x, y, z) = (T)std::stod(line);
    //                     }
    //                 }
    //             }

    //             return true;
    //         }
    //         else if (getExtension(filename) == "exr")
    //         {
    //             float *out;
    //             int tmp_w;
    //             int tmp_h;
    //             const char *err = nullptr;

    //             int ret = LoadEXR(&out, &tmp_w, &tmp_h, filename.c_str(), &err);
    //             if (ret != TINYEXR_SUCCESS)
    //             {
    //                 if (err)
    //                 {
    //                     std::cout << err << std::endl;
    //                     FreeEXRErrorMessage(err);
    //                 }
    //             }
    //             else
    //             {
    //                 resize(tmp_w, tmp_h);

    //                 for (int y = 0; y < h; ++y)
    //                 {
    //                     for (int x = 0; x < w; ++x)
    //                     {
    //                         for (int z = 0; z < 3; ++z)
    //                         {
    //                             operator()(x, y, z) = (T)out[4 * (x + y * w) + z];
    //                         }
    //                     }
    //                 }

    //                 free(out);
    //                 return true;
    //             }
    //         }
    //         else if (stbi_is_hdr(filename.c_str()))
    //         {
    //             float *pxls = stbi_loadf(filename.c_str(),
    //                                      &wid,
    //                                      &hei,
    //                                      &dep,
    //                                      3);
    //             if (pxls)
    //             {
    //                 resize(wid, hei);

    //                 for (int y = 0; y < h; ++y)
    //                 {
    //                     for (int x = 0; x < w; ++x)
    //                     {
    //                         for (int z = 0; z < 3; ++z)
    //                         {
    //                             operator()(x, y, z) = (T)pxls[3 * (x + y * w) + z];
    //                         }
    //                     }
    //                 }

    //                 stbi_image_free(pxls);
    //                 return true;
    //             }
    //             else
    //             {
    //                 throw std::runtime_error("Could not load HDR image");
    //             }
    //         }
    //         else
    //         {
    //             unsigned char *pxls = stbi_load(filename.c_str(),
    //                                             &wid,
    //                                             &hei,
    //                                             &dep,
    //                                             4);

    //             // std::cout << "DEP: " << dep << std::endl;
    //             if (pxls)
    //             {
    //                 resize(wid, hei);

    //                 for (int y = 0; y < h; ++y)
    //                 {
    //                     for (int x = 0; x < w; ++x)
    //                     {
    //                         for (int z = 0; z < 3; ++z)
    //                         {
    //                             // change all of this back!
    //                             // if (pxls[4 * (x + y * w) + 3] != 0.0)
    //                             byteToVal(pxls[3 * (x + y * w) + z], operator()(x, y, z));
    //                             // else
    //                             //     byteToVal(0.0, operator()(x, y, z));
    //                         }
    //                     }
    //                 }

    //                 stbi_image_free(pxls);
    //                 return true;
    //             }
    //             else
    //             {
    //                 throw std::runtime_error("Could not load LDR image");
    //             }
    //         }
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Error reading Image for file: \"" << filename << "\":\n\t"
    //                   << stbi_failure_reason() << std::endl;
    //         return false;
    //     }

    //     return false;
    // }

    // template <typename T>
    // bool RGBImage<T>::write(const std::string &filename)
    // {
    //     if (getExtension(filename) == "txt")
    //     {
    //         std::ofstream file(filename);
    //         file << width() << "\n";
    //         file << height() << "\n";

    //         for (int z = 0; z < 3; ++z)
    //         {
    //             for (int x = 0; x < w; ++x)
    //             {
    //                 for (int y = 0; y < h; ++y)
    //                 {
    //                     file << operator()(x, y, z) << "\n";
    //                 }
    //             }
    //         }

    //         file.close();
    //     }
    //     else
    //     {
    //         std::string extension = getExtension(filename);

    //         std::transform(extension.begin(),
    //                        extension.end(),
    //                        extension.begin(),
    //                        ::tolower);

    //         try
    //         {
    //             if (extension == "hdr")
    //             {
    //                 std::vector<float> pxls(h * w * 3, 1.f);
    //                 for (int x = 0; x < w; ++x)
    //                 {
    //                     for (int y = 0; y < h; ++y)
    //                     {
    //                         for (int z = 0; z < 3; ++z)
    //                         {
    //                             pxls[z + 3 * (x + y * w)] = (float)operator()(x, y, z);
    //                         }
    //                     }
    //                 }

    //                 if (!stb::stbi_write_hdr(filename.c_str(), w, h, 3, &pxls[0]))
    //                 {
    //                     throw std::runtime_error("Could not write HDR image");
    //                 }
    //             }
    //             else if (extension == "exr")
    //             {
    //                 EXRHeader header;
    //                 InitEXRHeader(&header);

    //                 EXRImage image;
    //                 InitEXRImage(&image);

    //                 image.num_channels = 3;

    //                 std::vector<float> images[3];
    //                 images[0].resize(w * h);
    //                 images[1].resize(w * h);
    //                 images[2].resize(w * h);

    //                 for (int i = 0; i < h; i++)
    //                 {
    //                     for (int j = 0; j < w; ++j)
    //                     {
    //                         images[0][i * w + j] = pixels[i * w + j].r;
    //                         images[1][i * w + j] = pixels[i * w + j].g;
    //                         images[2][i * w + j] = pixels[i * w + j].b;
    //                     }
    //                 }

    //                 float *image_ptr[3];
    //                 image_ptr[0] = &(images[2].at(0)); // B
    //                 image_ptr[1] = &(images[1].at(0)); // G
    //                 image_ptr[2] = &(images[0].at(0)); // R

    //                 image.images = (unsigned char **)image_ptr;
    //                 image.width = w;
    //                 image.height = h;

    //                 header.num_channels = 3;
    //                 header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
    //                 // Must be BGR(A) order, since most of EXR viewers expect this channel order.
    //                 strncpy(header.channels[0].name, "B", 255);
    //                 header.channels[0].name[strlen("B")] = '\0';
    //                 strncpy(header.channels[1].name, "G", 255);
    //                 header.channels[1].name[strlen("G")] = '\0';
    //                 strncpy(header.channels[2].name, "R", 255);
    //                 header.channels[2].name[strlen("R")] = '\0';

    //                 header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
    //                 header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);

    //                 for (int i = 0; i < header.num_channels; i++)
    //                 {
    //                     header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;          // pixel type of input image
    //                     header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
    //                 }

    //                 const char *err;
    //                 int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);

    //                 if (ret != TINYEXR_SUCCESS)
    //                 {
    //                     fprintf(stderr, "Save EXR err: %s\n", err);
    //                     return ret;
    //                 }

    //                 // std::cout << "succeeded: " << filename << std::endl;

    //                 free(header.channels);
    //                 free(header.pixel_types);
    //                 free(header.requested_pixel_types);

    //                 return true;
    //             }
    //             else if (extension == "png" ||
    //                      extension == "bmp" ||
    //                      extension == "tga" ||
    //                      extension == "jpg" ||
    //                      extension == "jpeg")
    //             {
    //                 int outC = 4;
    //                 std::vector<unsigned char> pxls(h * w * outC, 255);

    //                 for (int x = 0; x < w; ++x)
    //                 {
    //                     for (int y = 0; y < h; ++y)
    //                     {
    //                         int z;
    //                         for (z = 0; z < 3; ++z)
    //                         {
    //                             // 2.2 does not exactly match .exr for low pixel values... fix later
    //                             // pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, z), 1.0 / 2.2));
    //                             pxls[z + outC * (x + y * w)] = valToByte(operator()(x, y, z));
    //                         }
    //                         for (; z < 3; ++z)
    //                         {
    //                             // pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, 0), 1.0 / 2.2));
    //                             pxls[z + outC * (x + y * w)] = valToByte(operator()(x, y, z));
    //                         }
    //                     }
    //                 }

    //                 if (extension == "png")
    //                 {
    //                     if (!stb::stbi_write_png(filename.c_str(), w, h,
    //                                              outC, &pxls[0],
    //                                              sizeof(unsigned char) * w * outC))
    //                     {
    //                         throw std::runtime_error("Could not write PNG image.");
    //                     }
    //                 }
    //                 else if (extension == "bmp")
    //                 {
    //                     if (!stb::stbi_write_bmp(filename.c_str(), w, h,
    //                                              outC, &pxls[0]))
    //                     {
    //                         throw std::runtime_error("Could not write BMP image.");
    //                     }
    //                 }
    //                 else if (extension == "tga")
    //                 {
    //                     if (!stb::stbi_write_tga(filename.c_str(), w, h,
    //                                              outC, &pxls[0]))
    //                     {
    //                         throw std::runtime_error("Could not write TGA image.");
    //                     }
    //                 }
    //                 else if (extension == "jpg" || extension == "jpeg")
    //                 {
    //                     if (!stb::stbi_write_jpg(filename.c_str(), w, h,
    //                                              outC, &pxls[0], 100))
    //                     {
    //                         throw std::runtime_error("Could not write JPEG image.");
    //                     }
    //                 }
    //                 else
    //                 {
    //                     throw std::invalid_argument("Invalid extension");
    //                 }
    //             }
    //         }
    //         catch (const std::exception &e)
    //         {
    //             std::cout << "ERROR :: Image::write(" << filename << ") :: " << e.what() << std::endl;
    //         }
    //     }

    //     return true;
    // }

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

    // static bool image_read(const std::string &filename, RGBImage<double> &image);
    // static bool image_write(const std::string &filename, RGBImage<double> &image);
}
