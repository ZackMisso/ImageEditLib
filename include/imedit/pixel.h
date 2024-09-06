#pragma once

#include <tgmath.h>

namespace imedit
{
    // // THIS IS GROSS
    // typedef float Float;
    // // typedef double Float;

    template <typename T>
    struct Pixel
    {
        Pixel() : r(0.f), g(0.f), b(0.f) {}
        Pixel(T v) : r(v), g(v), b(v) {}
        Pixel(T r, T g, T b) : r(r), g(g), b(b) {}

        void convert_from_rgb_to_xyz()
        {
            // TODO
        }

        void convert_from_xyz_to_rgb()
        {
            // TODO
        }

        void convert_from_rgb_to_grey()
        {
            // TODO
        }

        void convert_grey_to_rgb()
        {
            // TODO
        }

        void clamp_non_negative()
        {
            if (r < (T)0.0)
                r = (T)0.0;
            if (g < (T)0.0)
                g = (T)0.0;
            if (b < (T)0.0)
                b = (T)0.0;
        }

        // TODO: maybe extend my vector implementation instead of rewriting
        //       all of this?

        T sum() const
        {
            return r + g + b;
        }

        Pixel<T> operator+(const Pixel<T> &other) const
        {
            return Pixel(r + other.r,
                         g + other.g,
                         b + other.b);
        }

        Pixel<T> operator-(const Pixel<T> &other) const
        {
            return Pixel(r - other.r,
                         g - other.g,
                         b - other.b);
        }

        Pixel<T> operator*(const Pixel<T> &other) const
        {
            return Pixel<T>(r * other.r,
                            g * other.g,
                            b * other.b);
        }

        Pixel<T> operator/(const Pixel<T> &other) const
        {
            return Pixel<T>(r / other.r,
                            g / other.g,
                            b / other.b);
        }

        Pixel<T> operator+(T val) const
        {
            return Pixel<T>(r + val,
                            g + val,
                            b + val);
        }

        Pixel<T> operator-(T val) const
        {
            return Pixel<T>(r - val,
                            g - val,
                            b - val);
        }

        Pixel<T> operator*(T val) const
        {
            return Pixel<T>(r * val,
                            g * val,
                            b * val);
        }

        Pixel<T> operator/(T val) const
        {
            return Pixel<T>(r / val,
                            g / val,
                            b / val);
        }

        void operator+=(const Pixel<T> &other)
        {
            r += other.r;
            g += other.g;
            b += other.b;
        }

        void operator-=(const Pixel<T> &other)
        {
            r -= other.r;
            g -= other.g;
            b -= other.b;
        }

        void operator*=(const Pixel<T> &other)
        {
            r *= other.r;
            g *= other.g;
            b *= other.b;
        }

        void operator/=(const Pixel<T> &other)
        {
            r /= other.r;
            g /= other.g;
            b /= other.b;
        }

        void operator+=(T val)
        {
            r += val;
            g += val;
            b += val;
        }

        void operator-=(T val)
        {
            r -= val;
            g -= val;
            b -= val;
        }

        void operator*=(T val)
        {
            r *= val;
            g *= val;
            b *= val;
        }

        void operator/=(T val)
        {
            r /= val;
            g /= val;
            b /= val;
        }

        bool operator!=(const Pixel<T> &other)
        {
            return r != other.r &&
                   g != other.g &&
                   b != other.b;
        }

        T min() const
        {
            return std::min(r, std::min(g, b));
        }

        T max() const
        {
            return std::max(r, std::max(g, b));
        }

        T &access(int index)
        {
            if (index % 3 == 0)
                return r;
            if (index % 3 == 1)
                return g;
            return b;
        }

        T const_access(int index) const
        {
            if (index % 3 == 0)
                return r;
            if (index % 3 == 1)
                return g;
            return b;
        }

        Pixel<T> positives() const
        {
            Pixel<T> pix = Pixel(0, 0, 0);

            if (r >= 0)
                pix.r = r;
            if (g >= 0)
                pix.g = g;
            if (b >= 0)
                pix.b = b;

            return pix;
        }

        Pixel<T> negatives() const
        {
            Pixel<T> pix = Pixel(0, 0, 0);

            if (r <= 0)
                pix.r = -r;
            if (g <= 0)
                pix.g = -g;
            if (b <= 0)
                pix.b = -b;

            return pix;
        }

        static Pixel<T> max(const Pixel<T> &one, const Pixel<T> &two)
        {
            Pixel<T> val;

            val.r = std::max(one.r, two.r);
            val.g = std::max(one.g, two.g);
            val.b = std::max(one.b, two.b);

            return val;
        }

        static Pixel<T> min(const Pixel<T> &one, const Pixel<T> &two)
        {
            Pixel<T> val;

            val.r = std::min(one.r, two.r);
            val.g = std::min(one.g, two.g);
            val.b = std::min(one.b, two.b);

            return val;
        }

        static Pixel<T> abs(const Pixel<T> &one)
        {
            Pixel<T> val;

            val.r = std::fabs(double(one.r));
            val.g = std::fabs(double(one.g));
            val.b = std::fabs(double(one.b));

            return val;
        }

        T r;
        T g;
        T b;
    };

}
