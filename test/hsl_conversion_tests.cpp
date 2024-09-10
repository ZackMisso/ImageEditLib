#include <iostream>
#include <tgmath.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

void write_pix(Pix pix)
{
    std::cout << "(" << pix.r << ", " << pix.g << ", " << pix.b << ")";
}

int main(int argc, char* argv[])
{
    Image image = Image(1024, 1024);

    for (int i = 0; i < 1024; ++i)
    {
        for (int j = 0; j < 1024; ++j)
        {
            float sat = 1.0 - float(i) / 1024.0;
            float hue = float(j) / 1024.0;

            Pix hsl = Pix(hue, sat, 0.5);
            if (i == 0)
                write_pix(hsl);
            imedit::hsl_to_rgb(hsl);
            if (i == 0)
            {
                std::cout << " -> ";
                write_pix(hsl);
                std::cout << std::endl;
            }

            image(j, i) = hsl;
        }
    }

    Pix test_1 = Pix(59.0 / 360.0, 1.0, 0.5);
    Pix test_2 = Pix(61.0 / 360.0, 1.0, 0.5);

    imedit::hsl_to_rgb(test_1);
    imedit::hsl_to_rgb(test_2);

    std::cout << "test:" << std::endl;
    write_pix(test_1);
    std::cout << std::endl;
    write_pix(test_2);

    imedit::write_image("hsl_mapping.exr", image);

    return 0;
}
