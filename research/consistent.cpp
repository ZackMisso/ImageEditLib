#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/consistent/images/";

    imedit::Image image = imedit::Image(800, 800);

    int iters = 8192;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            imedit::Image scene = imedit::Image(path + "path_" + std::to_string(i) + ".exr");

            if (scene.width() != 800) throw std::exception();

            image = image + (scene - image) * (1.0 / double(count+1));
            count++;

            if (count == 1) image.write("consist_1.exr");
            if (count == 2) image.write("consist_2.exr");
            if (count == 4) image.write("consist_4.exr");
            if (count == 8) image.write("consist_8.exr");
            if (count == 16) image.write("consist_16.exr");
            if (count == 32) image.write("consist_32.exr");
            if (count == 64) image.write("consist_64.exr");
            if (count == 128) image.write("consist_128.exr");
            if (count == 256) image.write("consist_256.exr");
            if (count == 512) image.write("consist_512.exr");
            if (count == 1024) image.write("consist_1024.exr");
            if (count == 2048) image.write("consist_2048.exr");
            if (count == 4096) image.write("consist_4096.exr");
        } catch(...) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    image.write("consistent_shader.exr");

    std::cout << "COUNT: " << count << std::endl;
}
