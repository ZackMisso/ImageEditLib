#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "imedit/im_color_maps.h"
#include "pcg/pcg32.h"

struct Complex2f
{
public:
    Complex2f() : real(0.0), imag(0.0) { }

    Complex2f(double real)
        : real(real), imag(0.0) { }

    Complex2f(double real, double imag)
        : real(real), imag(imag) { }

    Complex2f(const Complex2f& other)
        : real(other.real),
          imag(other.imag) { }

    double real;
    double imag;

    Complex2f operator+(double r) const
    {
        return Complex2f(real + r,
                         imag);
    }

    Complex2f operator-(double r) const
    {
        return Complex2f(real - r,
                         imag);
    }

    Complex2f operator+(const Complex2f& other) const
    {
        return Complex2f(real + other.real,
                         imag + other.imag);
    }

    Complex2f operator-(const Complex2f& other) const
    {
        return Complex2f(real - other.real,
                         imag - other.imag);
    }

    void operator+=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    void operator-=(const Complex2f& other)
    {
        real += other.real;
        imag += other.imag;
    }

    Complex2f operator*(const Complex2f& other) const
    {
        double r = real * other.real - imag * other.imag;
        double i = real * other.imag + imag * other.real;

        return Complex2f(r, i);
    }

    double norm() const
    {
        return real * real + imag * imag;
    }
};

int main(int argc, char* argv[])
{
    std::cout << "starting mandlebrot visualization" << std::endl;

    std::string publish_command = "ffmpeg -r 24 -f image2 -i mandlebrot_vis/mandlebrot_%04d.png -vcodec mpeg4 -vb 20M -minrate 20M -maxrate 30M mandlebrot_vis/mandlebrot.mp4";

    int res = 400;

    int frames = 1000;

    system("mkdir mandlebrot_vis");

    double max_real = 2.0;
    double min_real = -2.0;
    double max_imag = 2.0;
    double min_imag = -2.0;
    int max_iterations = 3000;
    double max_value = 10000.0;

    for (int k = 600; k < frames; ++k)
    {
        imedit::Image image = imedit::Image(res, res);
        std::cout << "creating frame: " << k << std::endl;

        for (int i = 0; i < res; ++i)
        {
            for (int j = 0; j < res; ++j)
            {
                double real = (max_real - min_real) * double(j) / double(res) + min_real;
                double imag = (max_imag - min_imag) * double(i) / double(res) + min_imag;

                Complex2f val = Complex2f(real, imag);

                int m = 1;
                Complex2f c = Complex2f();

                if (k < 200)
                {
                    c.real = -1.3 * double(k) / double(200);
                }
                else if (k < 400)
                {
                    double proxy = double(k - 200) / 199.0;

                    c.real = (1.0 - proxy) * -1.3 +
                             proxy * real;
                    c.imag = (1.0 - proxy) * 0.0 +
                             proxy * imag;
                }
                else if (k < 600)
                {
                    double proxy = double(k - 400) / 199.0;

                    c.real = (1.0 - proxy) * real +
                             proxy * -real;
                    c.imag = (1.0 - proxy) * imag +
                             proxy * -imag;
                }
                else if (k < 1000)
                {
                    double end_center_x = 0.517;
                    double end_center_y = 0.514;

                    double end_scale = 0.00001;

                    double proxy = double(k - 600) / 399.0;

                    double center_x = proxy * end_center_x;
                    double center_y = proxy * end_center_y;

                    double scale = end_scale * proxy + (1.0 - proxy) * 2.0;

                    double tmp_max_r = center_x + scale;
                    double tmp_min_r = center_x - scale;

                    double tmp_max_i = center_y + scale;
                    double tmp_min_i = center_y - scale;

                    // std::cout << "scale: " << scale << std::endl;

                    real = (tmp_max_r - tmp_min_r) * double(j) / double(res) + tmp_min_r;
                    imag = (tmp_max_i - tmp_min_i) * double(i) / double(res) + tmp_min_i;

                    val.real = real;
                    val.imag = imag;

                    c.real = -real;
                    c.imag = -imag;
                }

                while (val.norm() < max_value && m < max_iterations)
                {
                    m++;
                    val = val*val + c;
                }

                image(j, i) = double(m) / double(max_iterations);
            }
        }

        std::vector<imedit::Pixel> colors = std::vector<imedit::Pixel>();
        imedit::color_map_inferno(colors);

        // convert to color map
        for (int i = 0; i < res; ++i)
        {
            for (int j = 0; j < res; ++j)
            {
                double max_value = -log(1.0 / double(max_iterations));
                double value = -log(image(j, i).r) / max_value;
                image(j, i) = imedit::color_from_proxy(colors, value);
            }
        }

        char str[5];
        snprintf(str, 5, "%04d", k);
        std::string name = "mandlebrot_vis/mandlebrot_" + std::string(str);

        image.write(name + ".exr");
        image.write(name + ".png");
    }

    system(publish_command.c_str());

    return 0;
}
