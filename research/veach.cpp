#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/cluster_veach_path/images/";

    imedit::Image image = imedit::Image(512, 512);

    int iters = 2048;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 512) throw new std::exception();

            image = image + (scene - image) * (1.0 / double(count+1));
            count++;
        } catch(...) { std::cout << " iter failed" << std::endl; }

        // std::cout << "hey" << std::endl;
        std::cout << std::endl;
    }

    // fd_image.write("non_normed_fd.exr");
    image.write("veach_5_path.exr");
}
