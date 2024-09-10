#pragma once

#include "image.h"
#include <vector>

namespace imedit
{
    template <typename T>
    void sharpen3x3(const RGBImage<T> &old, RGBImage<T> &image)
    {
        // filter is of the form:
        // 0  -1  0
        // -1  4 -1
        // 0  -1  0

        for (int k = 0; k < image.depth(); ++k)
        {
            for (int i = 1; i < image.height() - 1; ++i)
            {
                for (int j = 1; j < image.width() - 1; ++j)
                {
                    T up_val = old.filter_index(j, i - 1, k);
                    T down_val = old.filter_index(j, i + 1, k);
                    T right_val = old.filter_index(j - 1, i, k);
                    T left_val = old.filter_index(j + 1, i, k);
                    T val = old.filter_index(j, i, k);

                    T new_val = -1.0 * (up_val + down_val + right_val + left_val) +
                                    4.0 * val;

                    image(j, i, k) = new_val;
                }
            }
        }
    }

    template <typename T>
    void xgrad3x1(const RGBImage<T> &old, RGBImage<T> &image)
    {
        // filter is of the form:
        // -1  0  1

        for (int k = 0; k < image.depth(); ++k)
        {
            for (int i = 0; i < image.height(); ++i)
            {
                for (int j = 0; j < image.width(); ++j)
                {
                    T right_val = old.filter_index(j + 1, i, k);
                    T left_val = old.filter_index(j - 1, i, k);

                    T new_val = -1.0 * left_val + right_val;

                    image(j, i, k) = new_val;
                }
            }
        }
    }

    template <typename T>
    void ygrad1x3(const RGBImage<T> &old, RGBImage<T> &image)
    {
        // filter is of the form:
        // [-1  0  1]^T

        for (int k = 0; k < image.depth(); ++k)
        {
            for (int i = 0; i < image.height(); ++i)
            {
                for (int j = 0; j < image.width(); ++j)
                {
                    T up_val = old.filter_index(j, i - 1, k);
                    T down_val = old.filter_index(j, i + 1, k);

                    T new_val = -1.0 * up_val + down_val;

                    image(j, i, k) = new_val;
                }
            }
        }
    }
}
