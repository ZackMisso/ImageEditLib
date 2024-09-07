/*
    ImageEditLib was developed by Zack Misso
    <zack441@mac.com>

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
    static void clamp_im(RGBImage<T> &image)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < 0.0)
                image[i] = 0.0;
            if (image[i] > 1.0)
                image[i] = 1.0;
        }
    }

    template <typename T>
    static void clamp_im(RGBImage<T> &image, T min, T max)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < min)
                image[i] = min;
            if (image[i] > max)
                image[i] = max;
        }
    }

    template <typename T>
    static void apply_mask(RGBImage<T> &image,
                           const RGBImage<T> &mask,
                           T threshold = 0.f,
                           T scale = 0.f)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (mask[i] < 0.f)
            {
                image[i] = 0.f;
            }
            else if (mask[i] < threshold)
            {
                image[i] *= scale;
            }
        }
    }

    template <typename T>
    static T clamp(T val)
    {
        if (val < 0.0)
            return 0.0;
        if (val > 1.0)
            return 1.0;
        return val;
    }

    template <typename T>
    static T clamp(T val, T min, T max)
    {
        if (val < min)
            return min;
        if (val > max)
            return max;
        return val;
    }

    template <typename T>
    static void remap_range_lin(RGBImage<T> &image)
    {
        T min = image.min();
        T max = image.max();

        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = (image[i] - min) / (max - min);
        }
    }

    template <typename T>
    static void exp_im(RGBImage<T> &image, T period)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = exp(image[i] * period);
        }
    }

    template <typename T>
    static void pow_im(RGBImage<T> &image, T exponent)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = pow(image[i], exponent);
        }
    }

    template <typename T>
    static void remap_range_lin(VEC(RGBImage<T>) & images)
    {
        T min = images[0].min();
        T max = images[0].max();

        for (int i = 1; i < images.size(); ++i)
        {
            T tmp_min = images[i].min();
            T tmp_max = images[i].max();

            if (min > tmp_min)
                min = tmp_min;
            if (max < tmp_max)
                max = tmp_max;
        }

        for (int i = 0; i < images.size(); ++i)
        {
            for (int j = 0; j < images[i].size(); ++j)
            {
                images[i][j] = (images[i][j] - min) / (max - min);
            }
        }
    }

    // continue from here
    template <typename T>
    static void remap_avg(RGBImage<T> &image, T new_avg)
    {
        T avg = image.average();

        for (int i = 0; i < image.size(); ++i)
        {
            image[i] -= avg;
        }
    }

    template <typename T>
    static void color_map_image(RGBImage<T> &image,
                                const VEC(Pixel<T>) & colors)
    {
        int len = colors.size() - 1;
        T step = 1.0 / T(len);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                T val = T(j) / T(image.width() - 1) / step;
                int index = int(val);
                T partial = val - (T)index;

                image(j, i, 0) = (1.0 - partial) * colors[index].r + partial * colors[index + 1].r;
                image(j, i, 1) = (1.0 - partial) * colors[index].g + partial * colors[index + 1].g;
                image(j, i, 2) = (1.0 - partial) * colors[index].b + partial * colors[index + 1].b;
            }
        }
    }

    template <typename T>
    static void color_map_ticks(std::vector<T> &ticks,
                                const std::vector<T> &stops,
                                int num_ticks)
    {
        T tick_step = 1.0 / T(num_ticks - 1);

        ticks.push_back(0.0);

        for (int i = 1; i < num_ticks - 1; ++i)
        {
            T tick_loc = tick_step * i;

            T stop_loc = (stops.size() - 1) * tick_loc;
            int stop_index = int(stop_loc);
            T stop_partial = stop_loc - (T)(stop_index);

            T tick_val = (1.0 - stop_partial) * T(stops[stop_index]) + (stop_partial) * double(stops[stop_index + 1]);

            ticks.push_back(tick_val);
        }

        ticks.push_back(1.0);
    }

    // TODO: create tone map functionality

    template <typename T>
    static void im_sin(RGBImage<T> &image)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = sin(image[i]);
        }
    }

    template <typename T>
    static void im_cos(RGBImage<T> &image)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = cos(image[i]);
        }
    }

    template <typename T>
    static void one_minus(RGBImage<T> &image)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            image[i] = (T)1.0 - image[i];
        }
    }

    template <typename T>
    static void thresh_min(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < threshold)
                image[i] = threshold;
        }
    }

    template <typename T>
    static void thresh_min_zero(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < threshold)
                image[i] = 0.0;
        }
    }

    template <typename T>
    static void thresh_min_image(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] > threshold)
                image[i] = 1.0;
            else
                image[i] = 0.0;
        }
    }

    template <typename T>
    static void thresh_max(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] > threshold)
                image[i] = threshold;
        }
    }

    template <typename T>
    static void thresh_max_zero(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] > threshold)
                image[i] = 0.0;
        }
    }

    template <typename T>
    static void thresh_max_image(RGBImage<T> &image, T threshold)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < threshold)
                image[i] = 1.0;
            else
                image[i] = 0.0;
        }
    }

    template <typename T>
    static void im_abs(RGBImage<T> &image)
    {
        for (int i = 0; i < image.size(); ++i)
        {
            if (image[i] < 0.0)
                image[i] = -image[i];
        }
    }

    // TODO: why is this a pointer???
    template <typename T>
    static RGBImage<T> *lerp(T t, const RGBImage<T> &one, const RGBImage<T> &two)
    {
        RGBImage<T> *img = new RGBImage<T>(one.width(), one.height());

        *img = one * (1.0 - t) + two * t;

        return img;
    }

    template <typename T>
    static void false_color_proxies(const RGBImage<T> &other,
                                    std::vector<T> &proxies,
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
            }
        }
    }

    // it is assumed the bins have already been given a size
    // and that size is larger than 1
    template <typename T>
    static void false_color_proxies_bins(const RGBImage<T> &other,
                                         std::vector<int> &bins,
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

    // // Note: this only applies a uniform color scaling
    // template <typename T>
    // void falseColor(const std::vector<T>& proxies,
    //                 const std::vector<Color3<T> >& colors,
    //                 std::vector<Color3<T> >& proxy_colors,
    //                 bool isConstant)
    // {
    //     // proxies are stored in [0,1]
    //     assert(colors.size() > 1);
    //
    //     double step = 1.0 / double(colors.size() - 1);
    //
    //     for (int i = 0; i < proxies.size(); ++i)
    //     {
    //         T val = proxies[i] / step;
    //         int index = std::floor(val);
    //         T partial = val - T(index);
    //
    //         if (index < 0)
    //         {
    //             proxy_colors.push_back(colors[0]);
    //             continue;
    //         }
    //         else if (index > colors.size()-2)
    //         {
    //             proxy_colors.push_back(colors[colors.size()-1]);
    //             continue;
    //         }
    //
    //         if (isConstant)
    //         {
    //             if (partial > 0.5)
    //             {
    //                 proxy_colors.push_back(colors[index + 1]);
    //             }
    //             else
    //             {
    //                 proxy_colors.push_back(colors[index]);
    //             }
    //         }
    //         else
    //         {
    //             Color3<T> tmp_color = Color3<T>
    //             (
    //                 (1.0 - partial) * colors[index].r + partial * colors[index + 1].r,
    //                 (1.0 - partial) * colors[index].g + partial * colors[index + 1].g,
    //                 (1.0 - partial) * colors[index].b + partial * colors[index + 1].b
    //             );
    //
    //             proxy_colors.push_back(tmp_color);
    //         }
    //     }
    // }

    // Note: this only applies a uniform color scaling
    template <typename T>
    static void falseColorProxy(const RGBImage<T> &other,
                                RGBImage<T> &image,
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
    static void histogram_grayscale(const RGBImage<T> &image,
                                    std::vector<int> &hist,
                                    T min = 0.0,
                                    T max = 1.0)
    {
        assert(hist.size() != 0);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                T val = (image(j, i, 0) / (max - min)) * hist.size();

                int index = std::floor(val);

                if (index < 0)
                    hist[0]++;
                else if (index >= hist.size())
                    hist[hist.size() - 1]++;
                else
                    hist[index]++;
            }
        }
    }

    template <typename T>
    static RGBImage<T> edge_image(const RGBImage<T> &image)
    {
        RGBImage<T> edge = RGBImage<T>(image.width(), image.height());

        for (int i = 1; i < edge.height() - 1; ++i)
        {
            for (int j = 1; j < edge.width() - 1; ++j)
            {
                edge(j, i, 0) = (image(j + 1, i, 0) - image(j - 1, i, 0) +
                                 image(j, i + 1, 0) - image(j, i - 1, 0)) /
                                4.0;
                edge(j, i, 1) = (image(j + 1, i, 1) - image(j - 1, i, 1) +
                                 image(j, i + 1, 1) - image(j, i - 1, 1)) /
                                4.0;
                edge(j, i, 2) = (image(j + 1, i, 2) - image(j - 1, i, 2) +
                                 image(j, i + 1, 2) - image(j, i - 1, 2)) /
                                4.0;
            }
        }

        return edge;
    }

    // // a false color scheme with arbitrary locations for all color stops
    // template <typename T>
    // void falseColor(const Image<T>& other,
    //                 Image<T>& image,
    //                 const std::vector<T>& stops,
    //                 const std::vector<Color3<T> >& colors,
    //                 T min,
    //                 T max,
    //                 bool isConstant)
    // {
    //     if (isConstant) std::cout << "ERROR: IS CONSTANT NOT SUPPORTED YET" << std::endl;
    //
    //     assert(colors.size() == stops.size());
    //
    //     Image<T> proxies = other;
    //
    //     falseColorProxy(other,
    //                     proxies,
    //                     min,
    //                     max);
    //
    //     for (int i = 0; i < proxies.height(); ++i)
    //     {
    //         for (int j = 0; j < proxies.width(); ++j)
    //         {
    //             T proxy = proxies(j, i, 0);
    //
    //             int k = 0;
    //             for (; k < stops.size(); ++k)
    //             {
    //                 if (proxy < stops[k]) break;
    //             }
    //
    //             if (k == 0)
    //             {
    //                 image(j, i, 0) = colors[0].r;
    //                 image(j, i, 1) = colors[0].g;
    //                 image(j, i, 2) = colors[0].b;
    //             }
    //             else if (k == stops.size())
    //             {
    //                 image(j, i, 0) = colors[colors.size() - 1].r;
    //                 image(j, i, 1) = colors[colors.size() - 1].g;
    //                 image(j, i, 2) = colors[colors.size() - 1].b;
    //             }
    //             else
    //             {
    //                 T up = stops[k];
    //                 T down = stops[k - 1];
    //
    //                 T t = (proxy - down) / (up - down);
    //
    //                 image(j, i, 0) = (1.0 - t) * colors[k - 1].r +
    //                                  t * colors[k].r;
    //                 image(j, i, 1) = (1.0 - t) * colors[k - 1].g +
    //                                  t * colors[k].g;
    //                 image(j, i, 2) = (1.0 - t) * colors[k - 1].b +
    //                                  t * colors[k].b;
    //             }
    //         }
    //     }
    // }
    //
    // // Note: this only applies a uniform color scaling
    // template <typename T>
    // void falseColor(const Image<T>& other,
    //                 Image<T>& image,
    //                 const std::vector<Color3<T> >& colors,
    //                 T min,
    //                 T max,
    //                 bool isConstant,
    //                 std::pair<T, T> range = std::pair<T, T>(0.0, 1.0))
    // {
    //     double step = 1.0 / double(colors.size() - 1);
    //
    //     // std::cout << "Min: " << min << std::endl;
    //     // std::cout << "Max: " << max << std::endl;
    //     //
    //     // std::cout << "LogMin: " << -std::log(min + 0.00000001) << std::endl;
    //     // std::cout << "LogMax: " << -std::log(max + 0.00000001) << std::endl;
    //     //
    //     // std::cout << "Range first: " << range.first << std::endl;
    //     // std::cout << "Range second: " << range.second << std::endl;
    //
    //     for (int i = 0; i < other.height(); ++i)
    //     {
    //         for (int j = 0; j < other.width(); ++j)
    //         {
    //             // other is assumed to be black and white so o[0] == o[1] == o[2]
    //             T val = other(j, i, 0);
    //
    //             // T logmin = -std::log(min + 0.0000000001);
    //             // T logmax = -std::log(max + 0.0000000001);
    //             // T logval = -std::log(val + 0.0000000001);
    //
    //             T logmin = -std::log(min + 0.0000000001);
    //             T logmax = -std::log(max + 0.0000000001);
    //             T logval = -std::log(val + 0.0000000001);
    //
    //             T partial = (logval - logmin) / (logmax - logmin);
    //
    //             // T partial = (val - min) / (max - min);
    //
    //             if (partial < range.first)
    //             {
    //                 image(j, i, 0) = colors[0].r;
    //                 image(j, i, 1) = colors[0].g;
    //                 image(j, i, 2) = colors[0].b;
    //             }
    //             else if (partial > range.second)
    //             {
    //                 image(j, i, 0) = colors[colors.size() - 1].r;
    //                 image(j, i, 1) = colors[colors.size() - 1].g;
    //                 image(j, i, 2) = colors[colors.size() - 1].b;
    //             }
    //             else
    //             {
    //                 double test_val = (((partial - range.first) / (range.second - range.first)) / step);
    //                 int index = std::floor(test_val);
    //
    //                 if (isConstant)
    //                 {
    //                     if (double(test_val) - double(index) > 0.5)
    //                     {
    //                         image(j, i, 0) = colors[index].r;
    //                         image(j, i, 1) = colors[index].g;
    //                         image(j, i, 2) = colors[index].b;
    //                     }
    //                     else
    //                     {
    //                         image(j, i, 0) = colors[index + 1].r;
    //                         image(j, i, 1) = colors[index + 1].g;
    //                         image(j, i, 2) = colors[index + 1].b;
    //                     }
    //                 }
    //                 else
    //                 {
    //                     double proxy = double(test_val) - double(index);
    //                     // if (proxy < 0.0) std::cout << "Uh Oh" << std::endl;
    //
    //                     image(j, i, 0) = (1.0 - proxy) * colors[index].r +
    //                                     proxy * colors[index + 1].r;
    //                                     image(j, i, 1) = (1.0 - proxy) * colors[index].g +
    //                                     proxy * colors[index + 1].g;
    //                                     image(j, i, 2) = (1.0 - proxy) * colors[index].b +
    //                                     proxy * colors[index + 1].b;
    //                 }
    //             }
    //         }
    //     }
    // }

    template <typename T>
    static RGBImage<T> *low_avg_comparison(const std::vector<RGBImage<T> *> &images)
    {
        T lowest = images[0]->average();
        RGBImage<T> *least_avg_image = images[0];

        for (int i = 1; i < images.size(); ++i)
        {
            T avg = images[i]->average();

            if (avg < lowest)
            {
                lowest = avg;
                least_avg_image = images[i];
            }
        }

        return least_avg_image;
    }

    template <typename T>
    static T mean_sqr_error(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;
        T size_term = 1.0 / one.size();

        for (int i = 0; i < one.size(); ++i)
        {
            err += (one[i] - two[i]) * (one[i] - two[i]);
        }

        err *= size_term;

        return err;
    }

    template <typename T>
    static T sqr_error(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;

        for (int i = 0; i < one.size(); ++i)
        {
            err = err + ((one[i] - two[i]) * (one[i] - two[i]));
        }

        return err;
    }

    template <typename T>
    static T error(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;

        for (int i = 0; i < one.size(); ++i)
        {
            err = err + std::abs(one[i] - two[i]);
        }

        return err;
    }

    template <typename T>
    static T root_mean_sqr_error(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;
        T size_term = 1.0 / one.size();

        for (int i = 0; i < one.size(); ++i)
        {
            err += (one[i] - two[i]) * (one[i] - two[i]);
        }

        err *= size_term;

        return sqrt(err);
    }

    template <typename T>
    static T mean_absolute_difference(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;
        T size_term = 1.0 / one.size();

        for (int i = 0; i < one.size(); ++i)
        {
            err += std::abs(one[i] - two[i]);
        }

        err *= size_term;

        return err;
    }

    template <typename T>
    static double mean_absolute_relative_difference(const RGBImage<T> &one, const RGBImage<T> &two)
    {
        T err = 0.0;
        T size_term = 1.0 / (T)one.size();

        for (int i = 0; i < one.size(); ++i)
        {
            err += std::abs(one[i] - two[i]) / one[i];
        }

        err *= size_term;

        return err;
    }

    // this is wrong
    template <typename T>
    static void hsl_to_rgb(Pixel<T> &pixel)
    {
        // hue is expected to be in the [0.0-1.0] range
        T hue = pixel.r * 360.0;
        T sat = pixel.g;
        T lum = pixel.b;

        T c = (1.0 - std::abs(2.0 * lum - 1.0)) * sat;
        T x = c * (1.0 - std::abs(std::fmod((hue / 60.0), 2.0) - 1.0));
        T m = lum - c / 2.0;

        T rp = 0.0;
        T gp = 0.0;
        T bp = 0.0;

        if (hue >= 0.0 && hue < 60.0)
        {
            rp = c;
            gp = x;
        }
        else if (hue >= 60.0 && hue < 120.0)
        {
            rp = x;
            gp = c;
        }
        else if (hue >= 120.0 && hue < 180.0)
        {
            gp = c;
            bp = x;
        }
        else if (hue >= 180.0 && hue < 240.0)
        {
            gp = x;
            bp = c;
        }
        else if (hue >= 240.0 && hue < 300.0)
        {
            bp = c;
            rp = x;
        }
        else if (hue >= 300.0 && hue < 360.0)
        {
            bp = x;
            rp = c;
        }

        pixel.r = rp + m;
        pixel.g = gp + m;
        pixel.b = bp + m;
    }

    // TODO: this is very slow
    template <typename T>
    static void minimize_neighbors(RGBImage<T> &image)
    {
        bool minimized_any = true;

        while (minimized_any)
        {
            minimized_any = false;

            for (int i = image.height() - 1; i >= 0; --i)
            {
                for (int j = image.width() - 1; j >= 0; --j)
                {
                    T old = image.safeAccess(j, i, 0);

                    if (old <= 1.f)
                    {
                        T min_val = fmin(
                            fmin(
                                image.safeAccess(j - 1, i, 0),
                                image.safeAccess(j + 1, i, 0)),
                            fmin(
                                image.safeAccess(j, i - 1, 0),
                                image.safeAccess(j, i + 1, 0)));

                        if (min_val < old)
                        {
                            image(j, i, 0) = min_val;
                            image(j, i, 1) = min_val;
                            image(j, i, 2) = min_val;

                            minimized_any = true;
                        }
                    }
                }
            }
        }
    }

    // this method assumes the input image has already been thresholded
    template <typename T>
    static RGBImage<T> group_white(const RGBImage<T> &image, int &count)
    {
        RGBImage<T> grouped = RGBImage<T>(image.width(), image.height());
        T max_val = image.width() * image.height() / 20;

        count = 1;
        T val = T(count) / max_val;

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                if (image(j, i, 0) == 1.f)
                {
                    T min_val = fmin(grouped.safeAccess(j - 1, i, 0),
                                     grouped.safeAccess(j, i - 1, 0));

                    if (min_val > 0.1f)
                    {
                        grouped(j, i, 0) = min_val;
                        grouped(j, i, 1) = min_val;
                        grouped(j, i, 2) = min_val;
                    }
                    else
                    {
                        std::cout << count << std::endl;
                        grouped(j, i, 0) = val;
                        grouped(j, i, 1) = val;
                        grouped(j, i, 2) = val;
                        count++;
                        val = T(count) / max_val;
                    }
                }
            }
        }

        // T max_val = image.width() * image.height();
        for (int i = 0; i < image.size(); ++i)
            if (grouped[i] < 1e-6)
                grouped[i] = 2.f;

        minimize_neighbors(grouped);

        for (int i = 0; i < image.size(); ++i)
            if (grouped[i] == 2.f)
                grouped[i] = 0.f;

        return grouped;
    }

    template <typename T>
    static void negpos_from_gray(RGBImage<T> &image)
    {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                if (image(j, i).r > 0.0)
                    image(j, i) = Pixel<T>(image(j, i).r, 0.0, 0.0);
                else
                    image(j, i) = Pixel<T>(0.0, 0.0, -image(j, i).r);
            }
        }
    }

    // returns an image mult times larger
    template <typename T>
    static RGBImage<T> mult_size(const RGBImage<T> &base, int mult)
    {
        RGBImage<T> image = RGBImage<T>(base.width() * mult, base.height() * mult);

        for (int i = 0; i < base.height(); ++i)
        {
            for (int j = 0; j < base.height(); ++j)
            {
                Pixel<T> val = base(j, i);
                int jj = mult * j;
                int ii = mult * i;

                for (int k = ii; k < ii + mult; ++k)
                {
                    for (int l = jj; l < jj + mult; ++l)
                    {
                        image(l, k) = val;
                    }
                }
            }
        }

        return image;
    }

    // returns an image mult times larger
    template <typename T>
    static RGBImage<T> resize_image(const RGBImage<T> &base, int width, int height)
    {
        RGBImage<T> image = RGBImage<T>(width, height);
        T w_factor = T(base.width()) / T(width);
        T h_factor = T(base.height()) / T(height);

        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.height(); ++j)
            {
                Pixel<T> val = Pixel<T>(0.0);
                T min_j = T(j) * w_factor;
                T max_j = T(j + 1) * w_factor;
                T min_i = T(i) * h_factor;
                T max_i = T(i + 1) * h_factor;
                int cnt = 0;

                for (int ii = std::floor(min_i); ii < std::ceil(max_i); ++ii)
                {
                    for (int jj = std::floor(min_j); jj < std::ceil(max_j); ++jj)
                    {
                        val += base(jj, ii);
                        cnt++;
                    }
                }

                val /= double(cnt);
                image(j, i) = val;
            }
        }

        return image;
    }

    template <typename T>
    static RGBImage<T> double_sized_image(const RGBImage<T> &input)
    {
        RGBImage<T> output = RGBImage<T>(input.width() * 2, input.height() * 2);

        for (int i = 0; i < output.height(); ++i)
        {
            for (int j = 0; j < output.width(); ++j)
            {
                output(j, i) = input(j / 2, i / 2);
            }
        }

        return output;
    }

}
