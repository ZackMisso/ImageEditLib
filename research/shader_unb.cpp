#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    std::string path = "/Users/corneria/Desktop/unb/shader_ball_unb_0_01/images/";
    std::string note_path = "/Users/corneria/Desktop/unb/shader_ball_unb_0_01/notes/";
    std::string name = "shader_unb";

    imedit::Image fd_image = imedit::Image(540, 540);

    int iters = 1024;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;

        try
        {
            std::ifstream notes;
            notes.open(note_path+"note_"+std::to_string(i)+".txt");

            double base_h;
            double n;
            double h;
            double hp1;
            double pmf;

            notes >> base_h;
            notes >> n;
            notes >> h;
            notes >> hp1;
            notes >> pmf;

            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << scene.width() << std::endl;
            imedit::Image basescene = imedit::Image(path + "base_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << basescene.width() << std::endl;
            imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what hp1 " << hscene.width() << std::endl;
            imedit::Image hp1scene = imedit::Image(path + "hp1_scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 540) throw std::exception();
            if (basescene.width() != 540) throw std::exception();
            if (hscene.width() != 540) throw std::exception();
            if (hp1scene.width() != 540) throw std::exception();

            imedit::Image chg = (basescene - scene) / base_h;
            imedit::Image dif = (hp1scene - scene) / hp1 - (hscene - scene) / h;
            imedit::Image eval = chg + dif / pmf;

            fd_image = fd_image + ((hscene - scene) - fd_image) * (1.0 / double(count+1));
            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    // fd_image.write(name + "_non_normed.exr");

    std::cout << "UNB COUNT: " << count << std::endl;
    fd_image.write(name + ".exr");
}
