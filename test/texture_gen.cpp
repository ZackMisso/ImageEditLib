#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"

using namespace std;
using namespace imedit;

float randomFloat()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

void noise_one()
{
    int res = 4096;
    double turb_period = double(res) / 64.0;
    double noise_period = double(res) / 16.0;
    double sharp_weight = 0.3;

    std::cout << "creating turb image" << std::endl;
    Image<double> turb_image = Image<double>(res, res, 3);
    turbulence_image_xy(turb_image, turb_period);
    turb_image.write("turb.hdr");
    remap_range_lin(turb_image);

    std::cout << "creating noise image" << std::endl;
    Image<double> noise_image = Image<double>(res, res, 3);
    noise_image_xy(noise_image, noise_period);
    im_abs(noise_image);

    // for (int i = 0; i < res; ++i)
    //     for (int j = 0; j < res; ++j)
    //     {
    //         if (j < res / 2)
    //         {
    //             noise_image(j, i, 0) = 1.0;
    //             noise_image(j, i, 1) = 0.0;
    //             noise_image(j, i, 2) = 0.0;
    //         }
    //         else
    //         {
    //             noise_image(j, i, 0) = 0.0;
    //             noise_image(j, i, 1) = 0.0;
    //             noise_image(j, i, 2) = 1.0;
    //         }
    //     }

    // noise_image.write("noise.hdr");

    std::cout << "creating sharp image" << std::endl;
    Image<double> sharp_image = Image<double>(res, res, 3);
    sharpen3x3(turb_image, sharp_image);
    remap_avg(sharp_image, 0.0);
    sharp_image = sharp_image * sharp_weight + turb_image;
    sharp_image.write("sharp.hdr");

    std::cout << "creating green image" << std::endl;
    Image<double> green_image = Image<double>(res, res, 3);
    green_image.setPixels(0.03, 0.1, 0.5);
    green_image *= 2.12;

    std::cout << "finishing up noise image" << std::endl;
    noise_image = noise_image * green_image * 2.0;
    noise_image.write("noise.hdr");

    std::cout << "creating sharp green image" << std::endl;
    green_image.write("green.hdr");
    green_image = green_image * sharp_image;
    double max = green_image.max();
    Image<double> max_image = Image<double>(res, res, 3);
    max_image.setPixels(max, max, max);
    green_image = max_image - green_image;
    green_image.write("green_sharp.hdr");
}

int main()
{
    srand(0x31245A);

    noise_one();

    return 0;
}
