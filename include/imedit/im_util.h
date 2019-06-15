/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

// This file contains functional transformations from one image to another,
// as well as transformations from multiple images to a single image.

#pragma once

#include <imedit/image.h>
#include <vector>
#include <tgmath.h>

namespace imedit
{

template <typename T>
void clamp_im(Image<T>& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < 0.0) image[i] = 0.0;
        if (image[i] > 1.0) image[i] = 1.0;
    }
}

template <typename T>
void clamp_im(Image<T>& image, T min, T max)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < min) image[i] = min;
        if (image[i] > max) image[i] = max;
    }
}

template <typename T>
T clamp(T val)
{
    if (val < 0.0) return 0.0;
    if (val > 1.0) return 1.0;
    return val;
}

template <typename T>
T clamp(T val, T min, T max)
{
    if (val < min) return min;
    if (val > max) return max;
    return val;
}

template <typename T>
void remap_range_lin(Image<T>& image)
{
    T min = image.min();
    T max = image.max();

    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = (image[i] - min) / (max - min);
    }
}

template <typename T>
void exp_im(Image<T>& image, T period)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = exp(image[i] * period);
    }
}

template <typename T>
void remap_range_lin(std::vector<Image<T> >& images)
{
    T min = images[0].min();
    T max = images[0].max();

    for (int i = 1; i < images.size(); ++i)
    {
        T tmp_min = images[i].min();
        T tmp_max = images[i].max();

        if (min > tmp_min) min = tmp_min;
        if (max < tmp_max) max = tmp_max;
    }

    for (int i = 0; i < images.size(); ++i)
    {
        for (int j = 0; j < images[i].size(); ++j)
        {
            images[i][j] = (images[i][j] - min) / (max - min);
        }
    }
}

template <typename T>
void remap_avg(Image<T>& image, T new_avg)
{
    T avg = image.average();

    for (int i = 0; i < image.size(); ++i)
    {
        image[i] -= avg;
    }
}

// TODO: create tone map functionality

template <typename T>
void im_sin(Image<T>& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = sin(image[i]);
    }
}

template <typename T>
void im_cos(Image<T>& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = cos(image[i]);
    }
}

template <typename T>
void one_minus(Image<T>& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        image[i] = (T)1.0 - image[i];
    }
}

template <typename T>
void thresh_min(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = threshold;
    }
}

template <typename T>
void thresh_min_zero(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = 0.0;
    }
}

template <typename T>
void thresh_min_image(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = 1.0;
        else image[i] = 0.0;
    }
}

template <typename T>
void thresh_max(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = threshold;
    }
}

template <typename T>
void thresh_max_zero(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] > threshold) image[i] = 0.0;
    }
}

template <typename T>
void thresh_max_image(Image<T>& image, T threshold)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < threshold) image[i] = 1.0;
        else image[i] = 0.0;
    }
}

template <typename T>
void im_abs(Image<T>& image)
{
    for (int i = 0; i < image.size(); ++i)
    {
        if (image[i] < 0.0) image[i] = -image[i];
    }
}

template <typename T>
Image<T>* lerp(T t, const Image<T>& one, const Image<T>& two)
{
    Image<T>* img = new Image<T>(one.width(), one.height(), one.depth());

    img = (1.0 - t) * one + t * two;

    return img;
}

template <typename T>
void falseColor(const Image<T>& other, Image<T>& image)
{
    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            T val = 0.212671 * other(j, i, 0) +
                    0.715160 * other(j, i, 1) +
                    0.072169 * other(j, i, 2);

            T r = clamp((val < 0.7) ? 4.0 * val - 1.5 : -4.0 * val + 4.5);
            T g = clamp((val < 0.5) ? 4.0 * val - 0.5 : -4.0 * val + 3.5);
            T b = clamp((val < 0.3) ? 4.0 * val + 0.5 : -4.0 * val + 2.5);

            image(j, i, 0) = r;
            image(j, i, 1) = g;
            image(j, i, 2) = b;
        }
    }
}

template <typename T>
Image<T>* low_avg_comparison(const std::vector<Image<T>*>& images)
{
    double lowest = images[0]->average();
    Image<T>* least_avg_image = images[0];

    for (int i = 1; i < images.size(); ++i)
    {
        double avg = images[i]->average();

        if (avg < lowest)
        {
            lowest = avg;
            least_avg_image = images[i];
        }
    }

    return least_avg_image;
}

template <typename T>
double mean_sqr_error(const Image<T>& one, const Image<T>& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += (one[i] - two[i]) * (one[i] - two[i]);
    }

    err *= size_term;

    return err;
}

template <typename T>
double root_mean_sqr_error(const Image<T>& one, const Image<T>& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += (one[i] - two[i]) * (one[i] - two[i]);
    }

    err *= size_term;

    return sqrt(err);
}

template <typename T>
double mean_absolute_difference(const Image<T>& one, const Image<T>& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += std::abs(one[i] - two[i]);
    }

    err *= size_term;

    return err;
}

template <typename T>
double mean_absolute_relative_difference(const Image<T>& one, const Image<T>& two)
{
    double err = 0.0;
    double size_term = 1.0 / one.size();

    for (int i = 0; i < one.size(); ++i)
    {
        err += std::abs(one[i] - two[i]) / one[i];
    }

    err *= size_term;

    return err;
}

void hsl_to_rgb(Pixel& pixel)
{
    // h=r; s=g; l=b
    Pixel new_pix;

    if (pixel.g == 0.0)
    {
        pixel.r = pixel.b;
        pixel.g = pixel.b;
        return;
    }

    double tmp_1 = 0.0;
    if (pixel.b < 0.5)
    {
        tmp_1 = pixel.b * (1.0 + pixel.g);
    }
    else
    {
        tmp_1 = pixel.b + pixel.g - (pixel.b * pixel.g);
    }

    double tmp_2 = 2.0 * pixel.b - tmp_1;

    double tmp_R = pixel.r + 0.3333;
    double tmp_G = pixel.r;
    double tmp_B = pixel.r - 0.3333;

    // convert the red channel
    if (tmp_R > 1.0 or tmp_R < 0.0)
    {
        new_pix.r = 0.0;
    }
    else if (6.0 * tmp_R < 1.0)
    {
        new_pix.r = (tmp_2 + (tmp_1 - tmp_2) * 6.0 * tmp_R);
    }
    else if (2.0 * tmp_R < 1.0)
    {
        new_pix.r = tmp_1;
    }
    else if (3.0 * tmp_R < 2.0)
    {
        new_pix.r = tmp_2 + (tmp_1 - tmp_2) * (0.667 - tmp_R) * 6.0;
    }
    else
    {
        new_pix.r = tmp_2;
    }

    // convert the green channel
    if (tmp_G > 1.0 or tmp_G < 0.0)
    {
        new_pix.g = 0.0;
    }
    else if (6.0 * tmp_G < 1.0)
    {
        new_pix.g = tmp_2 + (tmp_1 - tmp_2) * 6.0 * tmp_G;
    }
    else if (2.0 * tmp_G < 1.0)
    {
        new_pix.g = tmp_1;
    }
    else if (3.0 * tmp_G < 2.0)
    {
        new_pix.g = tmp_2 + (tmp_1 - tmp_2) * (0.667 - tmp_G) * 6.0;
    }
    else
    {
        new_pix.g = tmp_2;
    }

    // convert the blue channel
    if (tmp_B >= 1.0 or tmp_B <= 0.0)
    {
        new_pix.b = 0.0;
    }
    else if (6.0 * tmp_B < 1.0)
    {
        new_pix.b = tmp_2 + (tmp_1 - tmp_2) * 6.0 * tmp_B;
    }
    else if (2.0 * tmp_B < 1.0)
    {
        new_pix.b = tmp_1;
    }
    else if (3.0 * tmp_B < 2.0)
    {
        new_pix.b = tmp_2 + (tmp_1 - tmp_2) * (0.667 - tmp_B) * 6.0;
    }
    else
    {
        new_pix.b = tmp_2;
    }

    pixel.r = new_pix.r;
    pixel.g = new_pix.g;
    pixel.b = new_pix.b;
}


}
