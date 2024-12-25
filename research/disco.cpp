#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

int main()
{
    // std::string path = "/Users/corneria/Desktop/disco_ball/images/";
    std::string path = "/Users/corneria/Desktop/cluster_shader/images/";

    Image image = Image(540, 540);
    Image image_2 = Image(540, 540);

    int iters = 4096;
    int count = 0;

    // std::cout << "what"

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "iter: " << i;
        try
        {
            // imedit::Image scene = imedit::Image(path + "res_" + std::to_string(i) + ".exr");
            Image scene = Image(path + "scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 540) throw new std::exception();

            image = image + (scene - image) * 1.0 / double(count + 1);
            // image_2 = image_2 + scene;
            count++;
        } catch(std::exception e) { std::cout << " failed" << std::endl; }
        std::cout << std::endl;

        int tmp = iters;
        while (tmp > 1)
        {
            if (i == tmp)
                imedit::write_image("shader_"+std::to_string(i)+".exr", image);
            tmp /= 2;
        }
    }

    // image_2 /=

    imedit::write_image("woohoo.exr", image);
}
