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

// TODO: make these all static

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

template <typename T>
void color_map_image(Image<T>& image,
                     const std::vector<Color3<T> >& colors,
                     const std::vector<T>& stops)
{
    assert(colors.size() == stops.size());

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = T(j) / T(image.width()-1);

            for (int k = 0; k < stops.size(); ++k)
            {
                if (stops[k] >= val)
                {
                    T t = (val - stops[k-1]) / (stops[k] - stops[k - 1]);

                    image(j, i, 0) = (1.0 - t) * colors[k-1].r + t * colors[k].r;
                    image(j, i, 1) = (1.0 - t) * colors[k-1].g + t * colors[k].g;
                    image(j, i, 2) = (1.0 - t) * colors[k-1].b + t * colors[k].b;

                    break;
                }
            }
        }
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

// it is assumed the bins have already been given a size
// and that size is larger than 1
template <typename T>
void false_color_proxies_bins(const Image<T>& other,
                              std::vector<int>& bins,
                              T min,
                              T max,
                              std::pair<T, T> range = std::pair<T, T>(0.0, 1.0))
{
    assert(bins.size() > 1);

    T step = T(1.0) / T(bins.size() - 1);

    std::vector<T> proxies = std::vector<T>();

    false_color_proxies(other,
                        proxies,
                        min,
                        max);

    for (int i = 0; i < proxies.size(); ++i)
    {
        bins[std::floor((proxies[i] - range.first) / (range.second - range.first) / step)]++;
    }
}

template <typename T>
void false_color_proxies(const Image<T>& other,
                         std::vector<T>& proxies,
                         T min,
                         T max)
{
    T logmin = -std::log(min + 0.0000000001);
    T logmax = -std::log(max + 0.0000000001);

    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            T val = other(j, i, 0);

            T logval = -std::log(val + 0.0000000001);

            proxies.push_back((logval - logmin) / (logmax - logmin));

            // proxies.push_back((val - min) / (max - min));
        }
    }
}

// Note: this only applies a uniform color scaling
template <typename T>
void falseColor(const std::vector<T>& proxies,
                const std::vector<Color3<T> >& colors,
                std::vector<Color3<T> >& proxy_colors,
                bool isConstant)
{
    // proxies are stored in [0,1]
    assert(colors.size() > 1);

    double step = 1.0 / double(colors.size() - 1);

    for (int i = 0; i < proxies.size(); ++i)
    {
        T val = proxies[i] / step;
        int index = std::floor(val);
        T partial = val - T(index);

        if (index < 0)
        {
            proxy_colors.push_back(colors[0]);
            continue;
        }
        else if (index > colors.size()-2)
        {
            proxy_colors.push_back(colors[colors.size()-1]);
            continue;
        }

        if (isConstant)
        {
            if (partial > 0.5)
            {
                proxy_colors.push_back(colors[index + 1]);
            }
            else
            {
                proxy_colors.push_back(colors[index]);
            }
        }
        else
        {
            Color3<T> tmp_color = Color3<T>
            (
                (1.0 - partial) * colors[index].r + partial * colors[index + 1].r,
                (1.0 - partial) * colors[index].g + partial * colors[index + 1].g,
                (1.0 - partial) * colors[index].b + partial * colors[index + 1].b
            );

            proxy_colors.push_back(tmp_color);
        }
    }
}

// Note: this only applies a uniform color scaling
template <typename T>
void falseColorProxy(const Image<T>& other,
                     Image<T>& image,
                     T min,
                     T max,
                     std::pair<T, T> range = std::pair<T, T>(0.0, 1.0))
{
    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            // other is assumed to be black and white so o[0] == o[1] == o[2]
            T val = other(j, i, 0);

            T logmin = -std::log(min + 0.0000000001);
            T logmax = -std::log(max + 0.0000000001);
            T logval = -std::log(val + 0.0000000001);

            T partial = (logval - logmin) / (logmax - logmin);

            T proxy = partial;

            if (partial < range.first)
            {
                proxy = 0.0;
            }
            else if (partial > range.second)
            {
                proxy = 1.0;
            }
            else
            {
                proxy = (partial - range.first) / (range.second - range.first);
            }

            image(j, i, 0) = proxy;
            image(j, i, 1) = proxy;
            image(j, i, 2) = proxy;
        }
    }
}

template <typename T>
void histogram_grayscale(const Image<T>& image,
                         std::vector<int>& hist,
                         T min = 0.0,
                         T max = 1.0)
{
    assert(hist.size() != 0);

    // std::cout << "MIN: " << min << std::endl;
    // std::cout << "MAX: " << max << std::endl;

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = (image(j, i, 0) / (max - min)) * hist.size();

            int index = std::floor(val);

            if (index < 0) hist[0]++;
            else if (index >= hist.size()) hist[hist.size() - 1]++;
            else hist[index]++;
        }
    }
}

// a false color scheme with arbitrary locations for all color stops
template <typename T>
void falseColor(const Image<T>& other,
                Image<T>& image,
                const std::vector<T>& stops,
                const std::vector<Color3<T> >& colors,
                T min,
                T max,
                bool isConstant)
{
    if (isConstant) std::cout << "ERROR: IS CONSTANT NOT SUPPORTED YET" << std::endl;

    assert(colors.size() == stops.size());

    Image<T> proxies = other;

    falseColorProxy(other,
                    proxies,
                    min,
                    max);

    for (int i = 0; i < proxies.height(); ++i)
    {
        for (int j = 0; j < proxies.width(); ++j)
        {
            T proxy = proxies(j, i, 0);

            int k = 0;
            for (; k < stops.size(); ++k)
            {
                if (proxy < stops[k]) break;
            }

            if (k == 0)
            {
                image(j, i, 0) = colors[0].r;
                image(j, i, 1) = colors[0].g;
                image(j, i, 2) = colors[0].b;
            }
            else if (k == stops.size())
            {
                image(j, i, 0) = colors[colors.size() - 1].r;
                image(j, i, 1) = colors[colors.size() - 1].g;
                image(j, i, 2) = colors[colors.size() - 1].b;
            }
            else
            {
                T up = stops[k];
                T down = stops[k - 1];

                T t = (proxy - down) / (up - down);

                image(j, i, 0) = (1.0 - t) * colors[k - 1].r +
                                 t * colors[k].r;
                image(j, i, 1) = (1.0 - t) * colors[k - 1].g +
                                 t * colors[k].g;
                image(j, i, 2) = (1.0 - t) * colors[k - 1].b +
                                 t * colors[k].b;
            }
        }
    }
}

// Note: this only applies a uniform color scaling
template <typename T>
void falseColor(const Image<T>& other,
                Image<T>& image,
                const std::vector<Color3<T> >& colors,
                T min,
                T max,
                bool isConstant,
                std::pair<T, T> range = std::pair<T, T>(0.0, 1.0))
{
    double step = 1.0 / double(colors.size() - 1);

    // std::cout << "Min: " << min << std::endl;
    // std::cout << "Max: " << max << std::endl;
    //
    // std::cout << "LogMin: " << -std::log(min + 0.00000001) << std::endl;
    // std::cout << "LogMax: " << -std::log(max + 0.00000001) << std::endl;
    //
    // std::cout << "Range first: " << range.first << std::endl;
    // std::cout << "Range second: " << range.second << std::endl;

    for (int i = 0; i < other.height(); ++i)
    {
        for (int j = 0; j < other.width(); ++j)
        {
            // other is assumed to be black and white so o[0] == o[1] == o[2]
            T val = other(j, i, 0);

            // T logmin = -std::log(min + 0.0000000001);
            // T logmax = -std::log(max + 0.0000000001);
            // T logval = -std::log(val + 0.0000000001);

            T logmin = -std::log(min + 0.0000000001);
            T logmax = -std::log(max + 0.0000000001);
            T logval = -std::log(val + 0.0000000001);

            T partial = (logval - logmin) / (logmax - logmin);

            // T partial = (val - min) / (max - min);

            if (partial < range.first)
            {
                image(j, i, 0) = colors[0].r;
                image(j, i, 1) = colors[0].g;
                image(j, i, 2) = colors[0].b;
            }
            else if (partial > range.second)
            {
                image(j, i, 0) = colors[colors.size() - 1].r;
                image(j, i, 1) = colors[colors.size() - 1].g;
                image(j, i, 2) = colors[colors.size() - 1].b;
            }
            else
            {
                double test_val = (((partial - range.first) / (range.second - range.first)) / step);
                int index = std::floor(test_val);

                if (isConstant)
                {
                    if (double(test_val) - double(index) > 0.5)
                    {
                        image(j, i, 0) = colors[index].r;
                        image(j, i, 1) = colors[index].g;
                        image(j, i, 2) = colors[index].b;
                    }
                    else
                    {
                        image(j, i, 0) = colors[index + 1].r;
                        image(j, i, 1) = colors[index + 1].g;
                        image(j, i, 2) = colors[index + 1].b;
                    }
                }
                else
                {
                    double proxy = double(test_val) - double(index);
                    // if (proxy < 0.0) std::cout << "Uh Oh" << std::endl;

                    image(j, i, 0) = (1.0 - proxy) * colors[index].r +
                                    proxy * colors[index + 1].r;
                                    image(j, i, 1) = (1.0 - proxy) * colors[index].g +
                                    proxy * colors[index + 1].g;
                                    image(j, i, 2) = (1.0 - proxy) * colors[index].b +
                                    proxy * colors[index + 1].b;
                }
            }
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

static void hsl_to_rgb(Pixel& pixel)
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
