#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

void combine()
{
    Image one = Image("caustic_unb_1.exr");
    Image two = Image("caustic_unb_2.exr");
    Image three = Image("caustic_unb_3.exr");
    Image four =Image("caustic_unb_4.exr");

    Image image = one * (1003.0) + two * (994.0) + three * (1010.0) + four * (1021.0);
    double total = 1003.0 + 994.0 + 1010.0 + 1021.0;

    image /= total;

    imedit::write_image("caustic_unb_first.exr", image); // 4028
    std::cout << "total first: " << total << std::endl;
}

int main()
{
    std::string path = "/Users/corneria/Desktop/veach_unbiased/images/";
    std::string note_path = "/Users/corneria/Desktop/veach_unbiased/notes/";

    Image fd_image = Image(640, 360);

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

            Image scene = Image(path + "scene_" + std::to_string(i) + ".exr");
            Image basescene = Image(path + "base_scene_" + std::to_string(i) + ".exr");
            Image hscene = Image(path + "h_scene_" + std::to_string(i) + ".exr");
            Image hp1scene = Image(path + "hp1_scene_" + std::to_string(i) + ".exr");

            std::cout << "base_h: " << base_h << std::endl;

            if (scene.width() != 640) throw std::exception();
            if (basescene.width() != 640) throw std::exception();
            if (hscene.width() != 640) throw std::exception();
            if (hp1scene.width() != 640) throw std::exception();

            Image chg = (scene - basescene) / base_h;
            Image dif = (scene - hp1scene) / hp1 - (scene - hscene) / h;
            Image eval = dif / pmf;

            fd_image = fd_image * (double(count) / double(count+1)) + eval * (1.0 / double(count+1));

            // fd_image = fd_image + (eval/pmf - fd_image) * (1.0 / double(count+1));

            imedit::write_image("ind/"+std::to_string(i) + "_accum_" + std::to_string(int(n))+ ".exr", fd_image);
            imedit::write_image("ind/"+std::to_string(i) + "_eval_" + std::to_string(int(n))+ ".exr", eval);
            imedit::write_image("ind/"+std::to_string(i) + "_chg_" + std::to_string(int(n))+ ".exr", chg);

            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }

        std::cout << std::endl;
    }

    std::cout << "FINAL COUNT: " << count << std::endl;

    imedit::write_image("veach_fd.exr", fd_image);
}
