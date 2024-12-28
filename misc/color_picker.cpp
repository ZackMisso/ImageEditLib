#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/noise.h"
// #include "imedit/procedural.h"
#include "imedit/im_color_maps.h"
#include "imedit/filter.h"

#define Imaged imedit::RGBImage<double>
#define Pixd imedit::Pixel<double>

#define Imagef imedit::RGBImage<float>
#define Pixf imedit::Pixel<float>

#define TestImageRes 1000

void consistency_unit_test_float()
{
    system("mkdir color_picker_res/consist/");
    system("mkdir color_picker_res/consist/float/");
    std::string path = "color_picker_res/consist/float/";

    int wid = TestImageRes;
    int hei = TestImageRes;
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

    imedit::write_image(path + "test.exr", test);
    imedit::write_image(path + "test.png", test);

    Imagef test_2 = Imagef(path + "test.exr");

    imedit::write_image(path + "test_two.exr", test_2);
    imedit::write_image(path + "test_two.png", test_2);

    Imagef test_3 = Imagef(path + "test.png");

    imedit::write_image(path + "test_three.exr", test_3);
    imedit::write_image(path + "test_three.png", test_3);

    // consistency test
    Imagef consist_test = test;
    for (int i = 0; i < 16; ++i)
    {
        // good enough
        imedit::write_image(path + "consist_" + std::to_string(i) + ".exr", consist_test);
        imedit::write_image(path + "consist_" + std::to_string(i) + ".png", consist_test);

        imedit::read_image(path + "consist_" + std::to_string(i) + ".png", consist_test);
    }
}

void consistency_unit_test_double()
{
    system("mkdir color_picker_res/consist/");
    system("mkdir color_picker_res/consist/double/");
    std::string path = "color_picker_res/consist/double/";

    int wid = TestImageRes;
    int hei = TestImageRes;
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

    imedit::write_image(path + "test.exr", test);
    imedit::write_image(path + "test.png", test);

    Imaged test_2 = Imaged(path + "test.exr");

    imedit::write_image(path + "test_two.exr", test_2);
    imedit::write_image(path + "test_two.png", test_2);

    Imaged test_3 = Imaged(path + "test.png");

    imedit::write_image(path + "test_three.exr", test_3);
    imedit::write_image(path + "test_three.png", test_3);

    // consistency test
    Imaged consist_test = test;
    for (int i = 0; i < 16; ++i)
    {
        // good enough
        imedit::write_image(path + "consist_" + std::to_string(i) + ".exr", consist_test);
        imedit::write_image(path + "consist_" + std::to_string(i) + ".png", consist_test);

        imedit::read_image(path + "consist_" + std::to_string(i) + ".png", consist_test);
    }
}

void hue_box_visualization(float hue)
{
    std::string path = "color_picker_res/";

    float min_lum = 0.0f;
    float max_lum = 1.0f;
    float min_sat = 0.f;
    float max_sat = 1.f;

    int image_samples = 128;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Pixf pix = Pixf();
                pix.r = hue;
                pix.g = (float(i) + rng.nextFloat()) / float(image.height()) * (max_sat - min_sat) + min_sat;
                pix.b = (float(j) + rng.nextFloat()) / float(image.width()) * (max_lum - min_lum) + min_lum;

                imedit::hsl_to_rgb(pix);

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "hue_box.exr", image);
    imedit::write_image(path + "hue_box.png", image);
}

void lum_box_visualization(float lum)
{
    float min_hue = 0.f;
    float max_hue = 1.f;
    float min_sat = 0.0f;
    float max_sat = 1.0f;

    std::string path = "color_picker_res/";

    int image_samples = 128;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Pixf pix = Pixf();
                pix.b = lum;
                pix.g = 1.0 - (float(i) + rng.nextFloat()) / float(image.height()) * (max_sat - min_sat) + min_sat;
                pix.r = (float(j) + rng.nextFloat()) / float(image.width()) * (max_hue - min_hue) + min_hue;

                imedit::hsl_to_rgb(pix);

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "lum_box.exr", image);
    imedit::write_image(path + "lum_box.png", image);
}

void sat_box_visualization(float sat)
{
    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_hue = 0.f;
    float max_hue = 1.f;

    std::string path = "color_picker_res/";

    int image_samples = 32;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                Pixf pix = Pixf();
                pix.b = (float(i) + rng.nextFloat()) / float(image.height()) * (max_lum - min_lum) + min_lum;
                pix.g = sat;
                pix.r = (float(j) + rng.nextFloat()) / float(image.width()) * (max_hue - min_hue) + min_hue;

                imedit::hsl_to_rgb(pix);

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "sat_box.exr", image);
    imedit::write_image(path + "sat_box.png", image);
}

void hue_circle_visualization(float hue)
{
    // r param -     lum
    // theta param - sat
    float min_lum = 0.f;
    float max_lum = 1.f;
    float min_sat = 0.4f;
    float max_sat = 1.f;

    std::string path = "color_picker_res/";

    int image_samples = 128;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float r = 0.0;
                float theta = 0.0;

                float xx = (float(j) + rng.nextFloat() - float(image.width()) / 2.f);
                float yy = (float(i) + rng.nextFloat() - float(image.height()) / 2.f);

                // TODO: do these operations faster
                xx /= float(image.width()) / 2.f;
                yy /= float(image.height()) / 2.f;
                r = std::sqrt(xx*xx + yy*yy);
                if (yy != 0.0) {
                    theta = std::acos(xx / r) * yy / std::abs(yy);
                } else {
                    theta = std::acos(xx / r);
                }
                theta += M_PI;
                theta /= (2.f * M_PI);
                

                Pixf pix = Pixf();
                if (r <= 1.0) {
                    pix.g = r;
                    pix.g = pix.g * (max_sat - min_sat) + min_sat;
                    pix.r = hue;
                    pix.b = theta;
                    pix.b = pix.b * (max_lum - min_lum) + min_lum;
                    imedit::hsl_to_rgb(pix);
                } else {
                    pix = Pixf(0.0);
                }

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "hue_circ.exr", image);
    imedit::write_image(path + "hue_circ.png", image);
}

void lum_circle_visualization(float lum)
{
    // r param -     sat
    // theta param - hue
    float min_hue = 0.f;
    float max_hue = 1.f;
    float min_sat = 0.4f;
    float max_sat = 1.0;

    std::string path = "color_picker_res/";

    int image_samples = 128;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float r = 0.0;
                float theta = 0.0;

                float xx = (float(j) + rng.nextFloat() - float(image.width()) / 2.f);
                float yy = (float(i) + rng.nextFloat() - float(image.height()) / 2.f);

                // TODO: do these operations faster
                xx /= float(image.width()) / 2.f;
                yy /= float(image.height()) / 2.f;
                r = std::sqrt(xx*xx + yy*yy);
                if (yy != 0.0) {
                    theta = std::acos(xx / r) * yy / std::abs(yy);
                } else {
                    theta = std::acos(xx / r);
                }
                theta += M_PI;
                theta /= (2.f * M_PI);
                

                Pixf pix = Pixf();
                if (r <= 1.0) {
                    pix.g = r;
                    pix.g = pix.g * (max_sat - min_sat) + min_sat;
                    pix.b = lum;
                    pix.r = theta;
                    pix.r = pix.r * (max_hue - min_hue) + min_hue;
                    imedit::hsl_to_rgb(pix);
                } else {
                    pix = Pixf(0.0);
                }

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "lum_circ.exr", image);
    imedit::write_image(path + "lum_circ.png", image);
}

void sat_circle_visualization(float sat)
{
    // r param -     lum
    // theta param - hue

    float min_lum = 0.f;
    float max_lum = 0.6f;
    float min_hue = 0.f;
    float max_hue = 1.f;

    std::string path = "color_picker_res/";

    int image_samples = 128;
    int image_res = TestImageRes;

    pcg32 rng = pcg32(0x1234, 0x567a);

    Imagef image = Imagef(image_res, image_res);

    for (int k = 0; k < image_samples; ++k) {
        for (int i = 0; i < image.height(); ++i)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                float r = 0.0;
                float theta = 0.0;

                float xx = (float(j) + rng.nextFloat() - float(image.width()) / 2.f);
                float yy = (float(i) + rng.nextFloat() - float(image.height()) / 2.f);

                // TODO: do these operations faster
                xx /= float(image.width()) / 2.f;
                yy /= float(image.height()) / 2.f;
                r = std::sqrt(xx*xx + yy*yy);
                if (xx > r) {
                    std::cout << "WHAT THE FUFK" << std::endl;
                }
                if (yy != 0.0) {
                    theta = std::acos(xx / r) * yy / std::abs(yy);
                } else {
                    theta = std::acos(xx / r);
                }
                theta += M_PI;
                theta /= (2.f * M_PI);
                

                Pixf pix = Pixf();
                if (r <= 1.0) {
                    pix.b = 1.0 - r;
                    pix.b = pix.b * (max_lum - min_lum) + min_lum;
                    pix.g = sat;
                    pix.r = theta;
                    pix.r = pix.r * (max_hue - min_hue) + min_hue;
                    imedit::hsl_to_rgb(pix);
                } else {
                    pix = Pixf(0.0);
                }

                image(j, i) += pix / float(image_samples);
            }
        }
    }

    imedit::write_image(path + "sat_circ.exr", image);
    imedit::write_image(path + "sat_circ.png", image);
}

int main(int argc, char *argv[])
{
    system("rm -rf color_picker_res/consist/");
    consistency_unit_test_float();
    consistency_unit_test_double();

    std::cout << "creating hue box visualization" << std::endl;
    hue_box_visualization(0.2f);
    std::cout << "creating lum box visualization" << std::endl;
    lum_box_visualization(0.525f);
    std::cout << "creating sat box visualization" << std::endl;
    sat_box_visualization(1.0f);

    std::cout << "creating hue circle visualization" << std::endl;
    hue_circle_visualization(0.4f);
    std::cout << "creating lum circle visualization" << std::endl;
    lum_circle_visualization(0.525f);
    std::cout << "creating sat circle visualization" << std::endl;
    sat_circle_visualization(1.0f);

    return 0;
}
