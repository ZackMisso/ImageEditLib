#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

bool is_power_of_2(int val)
{
    int i = 1;
    while (i != 0)
    {
        if (val == i) return true;
        i <<= 1;
    }
    return false;
}

int main()
{
    std::string path = "/Users/corneria/Desktop/cluster_shader_unb/images/";
    std::string results = "shader_unbb/";
    system("mkdir shader_unbb/");

    imedit::Image image = imedit::Image(540, 540);

    int iters = 4096;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 540) throw std::exception();

            image = image + (scene - image) * (1.0 / double(count+1));

            count++;

            if (is_power_of_2(count))
            {
                image.write(results + "ps_"+std::to_string(count*64)+".exr");
            }
        } catch(...) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    image.write(results + "shader_unb.exr");

    std::cout << "unb COUNT: " << count << std::endl;
}
