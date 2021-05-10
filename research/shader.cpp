#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/fd_01/shader_ball_fd_0_01/images/";
    std::string name = "shader_fd_0_01";

    imedit::Image fd_image = imedit::Image(540, 540);

    int iters = 4096;
    int count = 0;
    double h = 0.01;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");
            imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 540) throw std::exception();
            if (hscene.width() != 540) throw std::exception();

            fd_image = fd_image + ((hscene - scene) - fd_image) * (1.0 / double(count+1));
            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    fd_image.write(name + "_non_normed.exr");

    std::cout << h << " COUNT: " << count << std::endl;
    fd_image = fd_image / h;
    fd_image.write(name + ".exr");
}
