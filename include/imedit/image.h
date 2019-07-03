/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>
#include <fstream>

#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_STATIC

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr/tinyexr.h>
// #include <tinyexr.h>

#pragma once

// useful functions

namespace imedit
{

typedef double Float;

struct Pixel
{
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
    Image() :
        w(0), h(0), d(0)
    {
        mode = IM_COLOR;
        im = std::vector<Float>();
    }

    Image(int w, int h, int d) :
        w(w), h(h), d(d)
    {
        if (d == 1) mode = IM_GREYSCALE;
        else if (d == 3) mode = IM_COLOR;
        else if (d % 3 == 0) mode = IM_3D_COLORED_TEXTURE;
        else mode = IM_3D_GRAYSCALE_TEXTURE;

        im = std::vector<Float>(w * h * d);
    }

    Image(int w, int h, int d, ImageMode mode) :
        w(w), h(h), d(d), mode(mode)
    {
        // TODO: assert mode
        im = std::vector<Float>(w * h * d);
    }

    Image(const std::string& filename)
    {
        read(filename);

        if (d == 1) mode = IM_GREYSCALE;
        else if (d == 3) mode = IM_COLOR;
        else if (d % 3 == 0) mode = IM_3D_COLORED_TEXTURE;
        else mode = IM_3D_GRAYSCALE_TEXTURE;
    }

    void clear()
    {
        for (int i = 0; i < size(); ++i)
        {
            im[i] = 0.0;
        }
    }

    // TODO: convert this form of averaging to be in-place
    double average() const
    {
        // use long double for precision
        long double avg = 0.0;

        for (int i = 0; i < size(); ++i)
        {
            avg += im[i];
        }

        avg /= (long double)size();

        return double(avg);
    }

    // TODO: convert this form of averaging to be in-place
    Pixel average_pixel() const
    {
        Pixel avg;
        avg.r = 0.0;
        avg.g = 0.0;
        avg.b = 0.0;

        for (int i = 0; i < height(); ++i)
        {
            for (int j = 0; j < width(); ++j)
            {
                avg.r += operator()(j, i, 0);
                avg.g += operator()(j, i, 1);
                avg.b += operator()(j, i, 2);
            }
        }

        avg.r /= double(size());
        avg.g /= double(size());
        avg.b /= double(size());

        return avg;
    }

    Float max() const
    {
        Float val = im[0];

        for (int i = 1; i < im.size(); ++i)
        {
            if (im[i] > val) val = im[i];
        }

        return val;
    }

    Float min() const
    {
        Float val = im[0];

        for (int i = 1; i < im.size(); ++i)
        {
            if (im[i] < val) val = im[i];
        }

        return val;
    }

    Pixel max_channel() const
    {
        Pixel max;

        max.r = operator()(0, 0, 0);
        max.g = operator()(0, 0, 1);
        max.b = operator()(0, 0, 2);

        for (int i = 0; i < height(); ++i)
        {
            for (int j = 0; j < width(); ++j)
            {
                if (operator()(j, i, 0) > max.r) max.r = operator()(j, i, 0);
                if (operator()(j, i, 1) > max.g) max.g = operator()(j, i, 1);
                if (operator()(j, i, 2) > max.b) max.b = operator()(j, i, 2);
            }
        }

        return max;
    }

    Pixel min_channel() const
    {
        Pixel min;

        min.r = operator()(0, 0, 0);
        min.g = operator()(0, 0, 1);
        min.b = operator()(0, 0, 2);

        for (int i = 0; i < height(); ++i)
        {
            for (int j = 0; j < width(); ++j)
            {
                if (operator()(j, i, 0) < min.r) min.r = operator()(j, i, 0);
                if (operator()(j, i, 1) < min.g) min.g = operator()(j, i, 1);
                if (operator()(j, i, 2) < min.b) min.b = operator()(j, i, 2);
            }
        }

        return min;
    }

    void brighten(Float factor)
    {
        (operator *=)(factor);
    }

    void contrast(Float factor, Float midpoint)
    {
        (operator -=)(midpoint);
        (operator *=)(factor);
        (operator +=)(midpoint);
    }

    void exposure(Float factor)
    {
        alterGamma(Float(1.0/2.2), Float(1.0));
        brighten(factor);
        alterGamma(Float(1.0), Float(1.0/2.2));
    }

    void alterGamma(Float oldGamma, Float newGamma)
    {
        T power = newGamma / oldGamma;
        for (int i = 0; i < im.size(); ++i)
        {
            im[i] = pow(im[i], power);
        }
    }

    // The read and write logic is based off of code written by Wojciech Jarosz
    bool read(const std::string& filename)
    {
        int wid;
        int hei;
        int dep;

        try
        {
            if (getExtension(filename) == "txt")
            {
                std::string line;
                std::ifstream file(filename);

                std::getline(file, line);
                int wid = std::stoi(line);
                std::getline(file, line);
                int hei = std::stoi(line);
                std::getline(file, line);
                int dep = std::stoi(line);

                resize(wid, hei, dep);

                for (int y = 0; y < h; ++y)
                {
                    for (int x = 0; x < w; ++x)
                    {
                        for (int z = 0; z < d; ++z)
                        {
                            std::getline(file, line);
                            operator()(x, y, z) = (T)std::stod(line);
                        }
                    }
                }

                return true;
            }
            else if (getExtension(filename) == "exr")
            {
                float* out;
                int tmp_w;
                int tmp_h;
                const char* err = nullptr;

                int ret = LoadEXR(&out, &tmp_w, &tmp_h, filename.c_str(), &err);
                if (ret != TINYEXR_SUCCESS)
                {
                    if (err)
                    {
                        std::cout << err << std::endl;
                        FreeEXRErrorMessage(err);
                    }
                }
                else
                {
                    resize(tmp_w, tmp_h, 3);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                operator()(x, y, z) = (Float)out[4 * (x + y * w) + z];
                            }
                        }
                    }

                    free(out);
                    return true;
                }
            }
            else if (stbi_is_hdr(filename.c_str()))
            {
                float* pxls = stbi_loadf(filename.c_str(),
                                         &wid,
                                         &hei,
                                         &dep,
                                         3);
                if (pxls)
                {
                    resize(wid, hei, dep);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                operator()(x, y, z) = (Float)pxls[3 * (x + y * w) + z];
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load HDR image");
                }
            }
            else
            {
                unsigned char* pxls = stbi_load(filename.c_str(),
                                                &wid,
                                                &hei,
                                                &dep,
                                                3);
                if (pxls)
                {
                    resize(wid, hei, dep);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                byteToVal(pxls[3 * (x + y * w) + z], operator()(x, y, z));
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load LDR image");
                }
            }
        }
        catch (const std::exception &e)
    	{
    		std::cerr << "Error reading Image for file: \"" << filename << "\":\n\t"
    		     << stbi_failure_reason() << std::endl;
    		return false;
    	}
    }

    bool write(const std::string& filename)
    {
        if (getExtension(filename) == "txt")
        {
            std::ofstream file(filename);
            file << width() << "\n";
            file << height() << "\n";
            file << depth() << "\n";

            for (int z = 0; z < d; ++z)
            {
                for (int x = 0; x < w; ++x)
                {
                    for (int y = 0; y < h; ++y)
                    {
                        file << operator()(x, y, z) << "\n";
                    }
                }
            }

            file.close();
        }
        else
        {
            if (d != 1 && d != 3 && d != 4)
            {
                std::cout << "Image must have 1, 3, or 4 channels" << std::endl;
                return false;
            }

            std::string extension = getExtension(filename);

            std::transform(extension.begin(),
                           extension.end(),
                           extension.begin(),
                           ::tolower);

            try
            {
                if (extension == "hdr")
                {
                    std::vector<float> pxls(h * w * 3, 1.f);
                    for (int x = 0; x < w; ++x)
                    {
                        for (int y = 0; y < h; ++y)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                pxls[z + 3 * (x + y * w)] = (float)operator()(x, y, z);
                            }
                        }
                    }

                    if (!stb::stbi_write_hdr(filename.c_str(), w, h, d, &pxls[0]))
                    {
                        throw std::runtime_error("Could not write HDR image");
                    }
                }
                else if (extension == "exr")
                {
                    EXRHeader header;
                    InitEXRHeader(&header);

                    EXRImage image;
                    InitEXRImage(&image);

                    image.num_channels = 3;

                    std::vector<float> images[3];
                    images[0].resize(w * h);
                    images[1].resize(w * h);
                    images[2].resize(w * h);

                    for (int i = 0; i < h; i++)
                    {
                        for (int j = 0; j < w; ++j)
                        {
                            images[0][i*w+j] = im[0*(w*h)+i*w+j];
                            images[1][i*w+j] = im[1*(w*h)+i*w+j];
                            images[2][i*w+j] = im[2*(w*h)+i*w+j];
                        }
                    }

                    float* image_ptr[3];
                    image_ptr[0] = &(images[2].at(0)); // B
                    image_ptr[1] = &(images[1].at(0)); // G
                    image_ptr[2] = &(images[0].at(0)); // R

                    image.images = (unsigned char**)image_ptr;
                    image.width = w;
                    image.height = h;

                    header.num_channels = 3;
                    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
                    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
                    strncpy(header.channels[0].name, "B", 255); header.channels[0].name[strlen("B")] = '\0';
                    strncpy(header.channels[1].name, "G", 255); header.channels[1].name[strlen("G")] = '\0';
                    strncpy(header.channels[2].name, "R", 255); header.channels[2].name[strlen("R")] = '\0';

                    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
                    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);

                    for (int i = 0; i < header.num_channels; i++)
                    {
                        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT; // pixel type of input image
                        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
                    }

                    const char* err;
                    int ret = SaveEXRImageToFile(&image, &header, filename.c_str(), &err);

                    if (ret != TINYEXR_SUCCESS)
                    {
                        fprintf(stderr, "Save EXR err: %s\n", err);
                        return ret;
                    }

                    free(header.channels);
                    free(header.pixel_types);
                    free(header.requested_pixel_types);

                    return true;
                }
                else if (extension == "png" ||
                         extension == "bmp" ||
                         extension == "tga" ||
                         extension == "jpg" ||
                         extension == "jpeg")
                {
                    int outC = 4;
                    std::vector<unsigned char> pxls(h * w * outC, 255);

                    for (int x = 0; x < w; ++x)
                    {
                        for (int y = 0; y < h; ++y)
                        {
                            int z;
                            for (z = 0; z < d; ++z)
                            {
                                pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, z), 1.0/2.2));
                            }
                            for (; z < 3; ++z)
                            {
                                pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, 0), 1.0/2.2));
                            }
                        }
                    }

                    if (extension == "png")
                    {
                        if (!stb::stbi_write_png(filename.c_str(), w, h,
        				                    outC, &pxls[0],
        				                    sizeof(unsigned char) * w * outC))
                        {
        					throw std::runtime_error("Could not write PNG image.");
                        }
                    }
                    else if (extension == "bmp")
                    {
                        if (!stb::stbi_write_bmp(filename.c_str(), w, h,
        				                    outC, &pxls[0]))
                        {
        					throw std::runtime_error("Could not write BMP image.");
                        }
                    }
                    else if (extension == "tga")
                    {
                        if (!stb::stbi_write_tga(filename.c_str(), w, h,
        				                    outC, &pxls[0]))
                        {
        					throw std::runtime_error("Could not write TGA image.");
                        }
                    }
                    else if (extension == "jpg" || extension == "jpeg")
                    {
                        if (!stb::stbi_write_jpg(filename.c_str(), w, h,
        				                    outC, &pxls[0], 100))
                        {
        					throw std::runtime_error("Could not write JPEG image.");
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("Invalid extension");
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "ERROR :: Image::write(" << filename << ") :: " << e.what() << std::endl;
            }
        }

        return true;
    }

    bool writeChannel(const std::string& filename, int ch)
    {
        return getChannel().write(filename);
    }

    void resize(int cols, int rows, int channels)
    {
        im.resize(cols * rows * channels);
        // TODO - not completely correct
        w = cols;
        h = rows;
        d = channels;
    }

    void setZero()
    {
        for (int i = 0; i < im.size(); ++i)
        {
            im[i] = 0;
        }
    }

    Image getChannel(int ch)
    {
        Image image = Image(w, h, 1);

        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                image(j, h, 0) = operator()(j, h, ch);
            }
        }

        return image;
    }

    void setPixels(T r, T b, T g)
    {
        if (d != 3) return;

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

    void setPixels(Pixel p)
    {
        if (d != 3) return;

        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                operator()(j, i, 0) = p.r;
                operator()(j, i, 1) = p.g;
                operator()(j, i, 2) = p.b;
            }
        }
    }

    void setPixel(int j, int i, Pixel p)
    {
        operator()(j, i, 0) = p.r;
        operator()(j, i, 1) = p.g;
        operator()(j, i, 2) = p.b;
    }

    // sort of a hack
    // void setDark(T val)
    // {
    //     for (int i = 0; i < h; ++i)
    //     {
    //         for (int j = 0; j < w; ++j)
    //         {
    //             T sum = 0.0;
    //             for (int k = 0; k < 3; ++k)
    //             {
    //                 sum += operator()(j, i, k);
    //             }
    //             if (sum < 0.05)
    //             {
    //                 operator()(j, i, 0) = val;
    //                 operator()(j, i, 1) = val;
    //                 operator()(j, i, 2) = val;
    //             }
    //         }
    //     }
    // }

    Image operator+(const Image& other) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + other[i];
        }

        return image;
    }

    Image<T> operator-(const Image<T>& other) const
    {
        Image<T> image = Image<T>(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - other[i];
        }

        return image;
    }

    // this is component-wise multiplication
    Image operator*(const Image& other) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * other[i];
        }

        return image;
    }

    // this is component-wise division
    Image operator/(const Image& other) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / other[i];
        }

        return image;
    }

    Image operator+(Float val) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) + val;
        }

        return image;
    }

    Image operator-(Float val) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) - val;
        }

        return image;
    }

    Image operator*(Float val) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) * val;
        }

        return image;
    }

    Image operator/(Float val) const
    {
        Image image = Image(w, h, d);

        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            image[i] = operator[](i) / val;
        }

        return image;
    }

    void operator+=(const Image& other)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += other[i];
        }
    }

    void operator-=(const Image& other)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= other[i];
        }
    }

    // component wise multiplication
    void operator*=(const Image& other)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= other[i];
        }
    }

    // component wise division
    void operator/=(const Image& other)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= other[i];
        }
    }

    void operator+=(Float val)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) += val;
        }
    }

    void operator-=(Float val)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) -= val;
        }
    }

    // component wise multiplication
    void operator*=(Float val)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) *= val;
        }
    }

    // component wise division
    void operator/=(Float val)
    {
        int size = w * h * d;

        for (int i = 0; i < size; ++i)
        {
            operator[](i) /= val;
        }
    }

    Float& operator[](int index)
    {
        return im[index];
    }

    Float operator[](int index) const
    {
        return im[index];
    }

    Float& operator()(int x, int y, int z)
    {
        // return im[z * w * h + y * w + x];
        return im[(z * h + y) * w + x];
    }

    Float operator()(int x, int y, int z) const
    {
        // return im[z * w * h + y * w + x];
        return im[(z * h + y) * w + x];
    }

    Float& filter_index(int x, int y, int z)
    {
        if (x < 0) x = 0;
        if (x > w-1) x = w-1;
        if (y < 0) y = 0;
        if (y > h-1) y = h-1;
        if (z < 0) z = 0;
        if (z > d-1) z = d-1;

        return im[(z * h + y) * w + x];
    }

    Float filter_index(int x, int y, int z) const
    {
        if (x < 0) x = 0;
        if (x > w-1) x = w-1;
        if (y < 0) y = 0;
        if (y > h-1) y = h-1;
        if (z < 0) z = 0;
        if (z > d-1) z = d-1;

        return im[(z * h + y) * w + x];
    }

    uint32_t width() const
    {
        return w;
    }

    uint32_t height() const
    {
        return h;
    }

    uint32_t depth() const
    {
        return d;
    }

    uint32_t size() const
    {
        return w * h * d;
    }

protected:
    std::vector<Float> im;

    ImageMode mode;
    uint32_t w, h, d;
};

}
