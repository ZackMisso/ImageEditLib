/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include "image.h"
#include "noise.h"

namespace imedit
{

// including z axis for now in case I want to extend this to support procedural
// voxel maps in the future
enum ImagePartition
{
    IP_X, // x axis only
    IP_Y, // y axis only
    // IP_Z, // z axis only

    // Should these be supported???
    // IP_R, // r channel only
    // IP_G, // g channel only
    // IP_B, // b channel only
    // IP_A, // a channel only

    IP_XY, // x and y axis
    // IP_XZ, // x and z axis
    // IP_YZ, // y and z axis
    // IP_XYZ, // all axis

    // should these be supported ???
    // IP_RG, // r and g channels only
    // IP_RB, // r and b channels only
    // IP_RA, // r and a channels only
    // IP_GB, // g and b channels only
    // IP_GA, // g and a channels only
    // IP_BA, // b and a channels only
    //
    // IP_RGB, // r g, and b channels only
    // IP_RGA, // r g, and a channels only
    // IP_RBA, // r b, and a channels only
    // IP_GBA, // g b, and a channels only
    // IP_RGBA // all channels
};

// void marble_image()

// TODO: maybe connect this to Vec implementation to use instead of pairs
template <typename T>
void euclidean_dist_from_points_image(Image<T>& image, std::vector<std::pair<T,T> > pairs)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = 10000000.0;

            for (int k = 0; k < pairs.size(); ++k)
            {
                T j_val = pairs[k].first - (T(j) + T(0.5));
                T i_val = pairs[k].second - (T(i) + T(0.5));

                j_val *= j_val;
                i_val *= i_val;

                T tmp = sqrt(j_val + i_val);

                if (tmp < val) val = tmp;
            }

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void manhattan_dist_from_points_image(Image<T>& image, std::vector<std::pair<int,int> > pairs)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int val = 10000000;

            for (int k = 0; k < pairs.size(); ++k)
            {
                int j_val = std::abs(pairs[k].first - j);
                int i_val = std::abs(pairs[k].second - i);

                int tmp = j_val + i_val;

                if (tmp < val) val = tmp;
            }

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void euclidean_tiled_image(Image<T>& image,
                           const std::vector<std::pair<T, T> >& pairs,
                           const std::vector<Pixel>& pixels)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int best_index = -1;
            T val = image.width() * image.height();

            for (int k = 0; k < pairs.size(); ++k)
            {
                T j_val = pairs[k].first - (T(j) + T(0.5));
                T i_val = pairs[k].second - (T(i) + T(0.5));

                j_val *= j_val;
                i_val *= i_val;

                T tmp = sqrt(j_val + i_val);

                if (tmp < val)
                {
                    val = tmp;
                    best_index = k;
                }
            }

            image.setPixel(j, i, pixels[best_index]);
        }
    }
}

template <typename T>
void manhattan_tiled_image(Image<T>& image,
                           const std::vector<std::pair<int,int> >& pairs,
                           const std::vector<Pixel>& pixels)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int val = image.width() * image.height();
            int best_index = -1;

            for (int k = 0; k < pairs.size(); ++k)
            {
                int j_val = std::abs(pairs[k].first - j);
                int i_val = std::abs(pairs[k].second - i);

                int tmp = j_val + i_val;

                if (tmp < val)
                {
                    val = tmp;
                    best_index = k;
                }
            }

            image.setPixel(j, i, pixels[best_index]);
        }
    }
}

template <typename T>
void euclidean_dist_image(Image<T>& image, T xpos, T ypos)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T j_val = xpos - (T(j) + T(0.5));
            T i_val = ypos - (T(i) + T(0.5));

            j_val *= j_val;
            i_val *= i_val;

            T val = sqrt(j_val + i_val);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void manhattan_dist_image(Image<T>& image, int xpos, int ypos)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int j_val = std::abs(xpos - j);
            int i_val = std::abs(ypos - i);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = j_val + i_val;
            }
        }
    }
}

// Note: I know this is inefficient - TODO: fix
template <typename T>
void splat_manhattan(Image<T>& image,
                     Pixel color,
                     int xpos,
                     int ypos,
                     int radius)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            int j_val = std::abs(xpos - j);
            int i_val = std::abs(ypos - i);

            if (j_val + i_val < radius)
            {
                image(j, i, 0) += color.r;
                image(j, i, 1) += color.g;
                image(j, i, 2) += color.b;
            }
        }
    }
}

// Note: I know this is inefficient - TODO: fix
template <typename T>
void splat_euclidean(Image<T>& image,
                     Pixel color,
                     T xpos,
                     T ypos,
                     T radius)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T j_val = xpos - (T(j) + T(0.5));
            T i_val = ypos - (T(i) + T(0.5));

            j_val *= j_val;
            i_val *= i_val;

            T val = sqrt(j_val + i_val);

            if (val < radius)
            {
                image(j, i, 0) += color.r;
                image(j, i, 1) += color.g;
                image(j, i, 2) += color.b;
            }
        }
    }
}

template <typename T>
void turbulence_image(Image<T>& image, T period)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = turbulence(T(j + 0.5), T(i + 0.5), T(k + 0.5), period);
            }
        }
    }
}

template <typename T>
void turbulence_image_xy(Image<T>& image, T period)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = turbulence(T(j + 0.5), T(i + 0.5), T(0.0), period);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void turbulence_image_xyz(Image<T>& image, T period, T z)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = turbulence(j + 0.5f, i + 0.5f, z + 0.5f, period);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void noise_image(Image<T>& image, T period)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = noise(j + 0.5f, i + 0.5f, k + 0.5f, period);
            }
        }
    }
}

template <typename T>
void noise_image_xy(Image<T>& image, T period, int xtrans=0.0, int ytrans=0.0)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            T val = noise(T(j + 0.5) + xtrans, T(i + 0.5) + ytrans, T(0.0), period);

            for (int k = 0; k < image.depth(); ++k)
            {
                image(j, i, k) = val;
            }
        }
    }
}

template <typename T>
void sin_image(T amplitude,
               T period,
               T phase,
               T offset,
               Image<T>& image,
               ImagePartition part)
{
    switch(part)
    {
        case IP_X:
        {
            for (int j = 0; j < image.width(); ++j)
            {
                T val = amplitude * sin(j / period + phase) + offset;

                for (int k = 0; k < image.depth(); ++k)
                {
                    for (int i = 0; i < image.height(); ++i)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        case IP_Y:
        {
            for (int i = 0; i < image.height(); ++i)
            {
                T val = amplitude * sin(i / period + phase) + offset;

                for (int k = 0; k < image.depth(); ++k)
                {
                    for (int j = 0; j < image.width(); ++j)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        // case IP_Z:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_R:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_G:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_B:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_A:
        // {
        //     // TODO
        //     break;
        // }
        case IP_XY:
        {
            for (int i = 0; i < image.height(); ++i)
            {
                for (int j = 0; j < image.width(); ++j)
                {
                    // TODO: how should this be implemented to support different
                    //       periods for x and y
                    T val = amplitude * sin((i + j) / period + phase) + offset;

                    for (int k = 0; k < image.depth(); ++k)
                    {
                        image(j, i, k) = val;
                    }
                }
            }

            break;
        }
        // case IP_XZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_YZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_XYZ:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RG:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_BA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGB:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RBA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_GBA:
        // {
        //     // TODO
        //     break;
        // }
        // case IP_RGBA:
        // {
        //     // TODO
        //     break;
        // }
    }
}

}
