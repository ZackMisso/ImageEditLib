#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/veach_biased/images/";

    imedit::Image fd_image = imedit::Image(1280, 720);

    int iters = 2048;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i << std::endl;
        // std::cout << "parsing scene" << std::endl;
        imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");
        // std::cout << "parsing h_scene" << std::endl;
        imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");

        // std::cout << scene.width() << std::endl;
        // std::cout << scene.height() << std::endl;

        // std::cout << "computing" << std::endl;
        fd_image = fd_image + ((hscene - scene) - fd_image) * (1.0 / double(i+1));
    }

    fd_image.write("non_normed_fd.exr");

    fd_image = fd_image / 0.01;
    fd_image.write("veach_fd.exr");
}
