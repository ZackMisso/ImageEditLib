#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

int main()
{
    std::string path = "/Users/corneria/Desktop/unb/shader_ball_unb_0_01/images/";
    std::string note_path = "/Users/corneria/Desktop/unb/shader_ball_unb_0_01/notes/";
    std::string name = "shader_unb";

    Image fd_image = Image(540, 540);

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

            std::cout << " hp1: " << hp1;
            std::cout << " pmf: " << pmf << std::endl;

            if (hp1 < 0.0001) throw std::exception();

            Image scene = Image(path + "scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << scene.width() << std::endl;
            Image basescene = Image(path + "base_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << basescene.width() << std::endl;
            Image hscene = Image(path + "h_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what hp1 " << hscene.width() << std::endl;
            Image hp1scene = Image(path + "hp1_scene_" + std::to_string(i) + ".exr");

            if (scene.width() != 540) throw std::exception();
            if (basescene.width() != 540) throw std::exception();
            if (hscene.width() != 540) throw std::exception();
            if (hp1scene.width() != 540) throw std::exception();

            Image chg = (basescene - scene) / base_h;
            Image dif = (hp1scene - scene) / hp1 - (hscene - scene) / h;
            Image eval = chg + dif / pmf;

            fd_image = fd_image + (eval - fd_image) * (1.0 / double(count+1));
            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }
        std::cout << std::endl;
    }

    // fd_image.write(name + "_non_normed.exr");

    std::cout << "UNB COUNT: " << count << std::endl;
    imedit::write_image(name + "_test_fuck_me.exr", fd_image);
}
