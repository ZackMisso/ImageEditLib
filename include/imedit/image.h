/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>
*/

#include <string>
#include <vector>
#include <stdexcept>
#include <stdio.h>
#include <string>
#include <iostream>

#include "stb_image.h"
#include "stb_image_write.h"

#pragma once

// useful functions

// clamps val between min and max
template <typename T>
inline T im_clamp(T val, T min, T max)
{
    return (val >= min) ?
           ((val <= max) ? val : max) : min;
}

//! Returns a modulus b.
template <typename T>
inline T im_mod(T a, T b)
{
    int n = (int)(a / b);
    a -= n * b;
    if (a < 0)
        a += b;
    return a;
}

// converts the value to a byte
template <typename T>
unsigned char valToByte(T val)
{
    T min = 0;
    T max = 1;
    return int(255.0 * im_clamp(val, min, max));
}

// converts the byte to a value
template <typename T>
T byteToVal(const unsigned char in)
{
    return in / T(255.0);
}

// returns the extension of a file path
std::string getExtension(const std::string &filename)
{
	if (filename.find_last_of(".") != std::string::npos)
		return filename.substr(filename.find_last_of(".") + 1);
	return "";
}

template <typename T>
class Image
{
public:
    Image() :
        w(0), h(0), d(0)
    {
        im = std::vector<T>();
    }

    Image(int w, int h, int d) :
        w(w), h(h), d(d)
    {
        im = std::vector<T>(w * h * d);
    }

    Image(const std::string& filename)
    {
        read(filename);
    }

    // The read and write logic is based off of code
    // written by Wojciech Jarosz
    bool read(const std::string& filename)
    {
        try
        {
            if (stbi_is_hdr(filename.c_str()))
            {
                float* pxls = stbi_loadf(filename.c_str(),
                                         &w,
                                         &h,
                                         &d,
                                         3);
                if (pxls)
                {
                    resize(w, h, d);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                operator()(x, y, z) = (T)pxls[3 * (x + y * w) + z];
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
                                                &w,
                                                &h,
                                                &d,
                                                3);
                if (pxls)
                {
                    resize(w, h, d);

                    for (int y = 0; y < h; ++y)
                    {
                        for (int x = 0; x < w; ++x)
                        {
                            for (int z = 0; z < d; ++z)
                            {
                                operator()(x, y, z) = (T)pxls[3 * (x + y * w) + z];
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
        if (d != 1 && d != 3 && d != 4)
        {
            std::cout << "Image must have 1, 3, or 4 channels" << std::endl;
            return false;
        }

        string extension = getExtension(filename);

        transform(extension.begin(),
                  extension.end(),
                  extension.begin(),
                  ::tolower);

        try
        {
            if (extension == "hdr")
            {
                // TODO
            }
            // TODO
        }
        catch (const std::exception &e)
        {
            // TODO
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

    Image<T> getChannel(int ch)
    {
        Image<T> image = Image<T>(w, h, 1);

        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                image(j, h, 0) = operator()(j, h, ch);
            }
        }

        return image;
    }

    Image<T> operator+(const Image<T>& other) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator-(const Image<T>& other) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator*(const Image<T>& other) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator/(const Image<T>& other) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator+(T val) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator-(T val) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator*(T val) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    Image<T> operator/(T val) const
    {
        Image<T> image = Image<T>(w, h, d);
        // TODO
        return image;
    }

    void operator+=(const Image<T>& other)
    {
        // Image<T> image = Image<T>();
        // // TODO
        // return image;
    }

    void operator-=(const Image<T>& other)
    {
        // Image<T> image = Image<T>();
        // // TODO
        // return image;
    }

    void operator*=(const Image<T>& other)
    {
        // Image<T> image = Image<T>();
        // // TODO
        // return image;
    }

    void operator/=(const Image<T>& other)
    {
        // Image<T> image = Image<T>();
        // // TODO
        // return image;
    }

    T& operator[](int index)
    {
        return im[index];
    }

    T operator()(int x, int y, int z)
    {
        // return im[z * w * h + y * w + x];
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

protected:
    std::vector<T> im;

    uint32_t w, h, d;
};
