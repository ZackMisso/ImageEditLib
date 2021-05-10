#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/caustic_yea/images/";

    imedit::Image fd_image = imedit::Image(512, 512);

    // a list of failures which have failed due to timing out.
    // std::vector<int> failures = std::vector<int>();
    // failures.push_back(73);
    // failures.push_back(258);
    // failures.push_back(463);
    // failures.push_back(653);
    // failures.push_back(859);
    // failures.push_back(1050);
    // failures.push_back(1219);
    // failures.push_back(1270);
    // failures.push_back(1500);
    // failures.push_back(1890);
    // failures.push_back(1773);
    // failures.push_back(2533);
    // failures.push_back(2134);
    // failures.push_back(3001);
    // failures.push_back(2356);
    // failures.push_back(3762);
    // failures.push_back(2635);
    // failures.push_back(2932);
    // failures.push_back(3237);
    // failures.push_back(3573);
    // failures.push_back(3931);

    int iters = 1024; // - 21
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        // bool skip = false;
        //
        // for (int j = 0; j < failures.size(); ++j)
        // {
        //     if (i == failures[j]) skip = true;
        // }

        try
        {

        // if (!skip)
        // {
            // std::cout << path + "scene_" + std::to_string(i) + ".exr" << std::endl;
            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");
            // std::cout << path + "h_scene_" + std::to_string(i) + ".exr" << std::endl;
            imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");
            // std::cout << hscene.width() << std::endl;

            if (scene.width() != 1024) throw std::exception();
            if (hscene.width() != 1024) throw std::exception();

            fd_image = fd_image + ((hscene - scene) - fd_image) * (1.0 / double(count+1));
            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    fd_image.write("non_normed_caustic_fd_yea.exr");

    std::cout << "YEA COUNT: " << count << std::endl;
    fd_image = fd_image / 0.1;
    fd_image.write("caustic_fd_yea.exr");
}
