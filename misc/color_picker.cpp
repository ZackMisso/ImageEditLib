#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
// #include "imedit/im_util.h"
// #include "imedit/procedural.h"
// #include "imedit/im_color_maps.h"

#define Imaged imedit::Image<double>
#define Pixd imedit::Pixel<double>

#define Imagef imedit::Image<float>
#define Pixf imedit::Pixel<float>

int main(int argc, char *argv[])
{
    system("mkdir color_picker_res/");
    std::string path = "color_picker_res/";

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
        // TODO: fix this consistency bug
        imedit::image_write(path + "consist_" + std::to_string(i) + ".exr", consist_test);
        imedit::image_write(path + "consist_" + std::to_string(i) + ".png", consist_test);

        imedit::image_read(path + "consist_" + std::to_string(i) + ".png", consist_test);
    }

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
