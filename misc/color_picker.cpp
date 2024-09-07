#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
// #include "imedit/procedural.h"
// #include "imedit/im_color_maps.h"

#define Imaged imedit::RGBImage<double>
#define Pixd imedit::Pixel<double>

#define Imagef imedit::RGBImage<float>
#define Pixf imedit::Pixel<float>

void consistency_unit_test_float()
{
    system("mkdir color_picker_res/consist/");
    system("mkdir color_picker_res/consist/float/");
    std::string path = "color_picker_res/consist/float/";

    int wid = 512;
    int hei = 512;
    Imagef test = Imagef(wid, hei);

    for (int i = 0; i < hei; ++i)
    {
        for (int j = 0; j < wid; ++j)
        {
            float x = float(j) / float(wid);
            float y = float(i) / float(hei);
            test(j, i) = Pixf(x, y, std::max(x, y));
        }
    }

    imedit::image_write(path + "test.exr", test);
    imedit::image_write(path + "test.png", test);

    Imagef test_2 = Imagef(path + "test.exr");

    imedit::image_write(path + "test_two.exr", test_2);
    imedit::image_write(path + "test_two.png", test_2);

    Imagef test_3 = Imagef(path + "test.png");

    imedit::image_write(path + "test_three.exr", test_3);
    imedit::image_write(path + "test_three.png", test_3);

    // consistency test
    Imagef consist_test = test;
    for (int i = 0; i < 16; ++i)
    {
        // good enough
        imedit::image_write(path + "consist_" + std::to_string(i) + ".exr", consist_test);
        imedit::image_write(path + "consist_" + std::to_string(i) + ".png", consist_test);

        imedit::image_read(path + "consist_" + std::to_string(i) + ".png", consist_test);
    }
}

void consistency_unit_test_double()
{
    system("mkdir color_picker_res/consist/");
    system("mkdir color_picker_res/consist/double/");
    std::string path = "color_picker_res/consist/double/";

    int wid = 512;
    int hei = 512;
    Imaged test = Imaged(wid, hei);

    for (int i = 0; i < hei; ++i)
    {
        for (int j = 0; j < wid; ++j)
        {
            double x = double(j) / double(wid);
            double y = double(i) / double(hei);
            test(j, i) = Pixd(x, y, std::max(x, y));
        }
    }

    imedit::image_write(path + "test.exr", test);
    imedit::image_write(path + "test.png", test);

    Imaged test_2 = Imaged(path + "test.exr");

    imedit::image_write(path + "test_two.exr", test_2);
    imedit::image_write(path + "test_two.png", test_2);

    Imaged test_3 = Imaged(path + "test.png");

    imedit::image_write(path + "test_three.exr", test_3);
    imedit::image_write(path + "test_three.png", test_3);

    // consistency test
    Imaged consist_test = test;
    for (int i = 0; i < 16; ++i)
    {
        // good enough
        imedit::image_write(path + "consist_" + std::to_string(i) + ".exr", consist_test);
        imedit::image_write(path + "consist_" + std::to_string(i) + ".png", consist_test);

        imedit::image_read(path + "consist_" + std::to_string(i) + ".png", consist_test);
    }
}

void hue_box_visualization(float hue)
{
    std::string path = "color_picker_res/";

    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_sat = 0.f;
    float max_sat = 1.f;

    int image_samples = 1;
    int image_res = 512;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(512, 512);

    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            Pixf pix = Pixf();
            pix.r = hue;
            pix.g = float(j) / float(image.width()) + rng.nextFloat() / float(image.width());
            pix.b = float(i) / float(image.height()) + rng.nextFloat() / float(image.height());
            imedit::hsl_to_rgb(pix);

            image(j, i) += pix / float(image_samples);
        }
    }

    imedit::image_write(path + "hue_box.exr", image);
    imedit::image_write(path + "hue_box.png", image);
}

void lum_box_visualization(float lum)
{
    float min_hue = 0.f;
    float max_hue = 1.f;
    float min_sat = 0.f;
    float max_sat = 1.f;

    // TODO
}

void sat_box_visualization(float sat)
{
    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_hue = 0.f;
    float max_hue = 1.f;

    // TODO
}

void hue_circle_visualization(float hue)
{
    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_sat = 0.f;
    float max_sat = 1.f;

    // TODO
}

void lum_circle_visualization(float lum)
{
    float min_hue = 0.f;
    float max_hue = 359.f;
    float min_sat = 0.f;
    float max_sat = 1.f;

    // TODO
}

void sat_circle_visualization(float sat)
{
    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_hue = 0.f;
    float max_hue = 359.f;

    // TODO
}

int main(int argc, char *argv[])
{
    system("rm -rf color_picker_res/consist/");
    consistency_unit_test_float();
    consistency_unit_test_double();

    // TODO: box
    // TODO: make hue visualization
    hue_box_visualization(0.0f);
    // TODO: make lum visualization
    lum_box_visualization(0.7f);
    // TODO: make sat visualization
    sat_box_visualization(0.7f);

    // TODO: circle
    // TODO: make hue visualization
    hue_circle_visualization(0.0f);
    // TODO: make lum visualization
    lum_circle_visualization(0.7f);
    // TODO: make sat visualization
    sat_circle_visualization(0.7f);

    // int start_frame = 0;
    // int end_frame = 4920;

    // std::string prefix = "/Users/corneria/Documents/Projects/FeignRenderer/build/hall_of_tiles_final/hall_of_tiles_final_";

    // for (int i = start_frame; i < end_frame; ++i)
    // {
    //     char str[5];
    //     snprintf(str, 5, "%04d", i);
    //     std::string frame = std::string(str);
    //     Imagef image;
    //     bool success = imedit::image_read(frame, image);
    //     if (!success)
    //         std::cout << frame << std::endl;
    // }

    return 0;
}
