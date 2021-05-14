#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

void combine()
{
    imedit::Image one = imedit::Image("caustic_unb_1.exr");
    imedit::Image two = imedit::Image("caustic_unb_2.exr");
    imedit::Image three = imedit::Image("caustic_unb_3.exr");
    imedit::Image four = imedit::Image("caustic_unb_4.exr");

    imedit::Image image = one * (1003.0) + two * (994.0) + three * (1010.0) + four * (1021.0);
    double total = 1003.0 + 994.0 + 1010.0 + 1021.0;

    image /= total;

    image.write("caustic_unb_first.exr"); // 4028
    std::cout << "total first: " << total << std::endl;
}

int main()
{
    std::string path = "/Users/corneria/Desktop/veach_unbiased/images/";
    std::string note_path = "/Users/corneria/Desktop/veach_unbiased/notes/";

    imedit::Image fd_image = imedit::Image(640, 360);

    int iters = 2048;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i << std::endl;

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
            imedit::Image basescene = imedit::Image(path + "base_scene_" + std::to_string(i) + ".exr");
            imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");
            imedit::Image hp1scene = imedit::Image(path + "hp1_scene_" + std::to_string(i) + ".exr");

            std::cout << "base_h: " << base_h << std::endl;

            if (scene.width() != 640) throw std::exception();
            if (basescene.width() != 640) throw std::exception();
            if (hscene.width() != 640) throw std::exception();
            if (hp1scene.width() != 640) throw std::exception();

            imedit::Image chg = (scene - basescene) / base_h;
            imedit::Image dif = (scene - hp1scene) / hp1 - (scene - hscene) / h;
            imedit::Image eval = dif / pmf;

            fd_image = fd_image * (double(count) / double(count+1)) + eval * (1.0 / double(count+1));

            // fd_image = fd_image + (eval/pmf - fd_image) * (1.0 / double(count+1));

            fd_image.write("ind/"+std::to_string(i) + "_accum_" + std::to_string(int(n))+ ".exr");
            eval.write("ind/"+std::to_string(i) + "_eval_" + std::to_string(int(n))+ ".exr");
            chg.write("ind/"+std::to_string(i) + "_chg_" + std::to_string(int(n))+ ".exr");

            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }

        std::cout << std::endl;
    }

    std::cout << "FINAL COUNT: " << count << std::endl;

    fd_image.write("veach_fd.exr");
}
