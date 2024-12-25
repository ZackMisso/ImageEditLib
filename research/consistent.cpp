#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

int main()
{
    std::string path = "/Users/corneria/Desktop/consistent/images/";

    Image image = Image(800, 800);

    int iters = 8192;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            Image scene = Image(path + "path_" + std::to_string(i) + ".exr");

            if (scene.width() != 800) throw std::exception();

            image = image + (scene - image) * (1.0 / double(count+1));
            count++;

            if (count == 1) imedit::write_image("consist_1.exr", image);
            if (count == 2) imedit::write_image("consist_2.exr", image);
            if (count == 4) imedit::write_image("consist_4.exr", image);
            if (count == 8) imedit::write_image("consist_8.exr", image);
            if (count == 16) imedit::write_image("consist_16.exr", image);
            if (count == 32) imedit::write_image("consist_32.exr", image);
            if (count == 64) imedit::write_image("consist_64.exr", image);
            if (count == 128) imedit::write_image("consist_128.exr", image);
            if (count == 256) imedit::write_image("consist_256.exr", image);
            if (count == 512) imedit::write_image("consist_512.exr", image);
            if (count == 1024) imedit::write_image("consist_1024.exr", image);
            if (count == 2048) imedit::write_image("consist_2048.exr", image);
            if (count == 4096) imedit::write_image("consist_4096.exr", image);
        } catch(...) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    imedit::write_image("consistent_shader.exr", image);

    std::cout << "COUNT: " << count << std::endl;
}
