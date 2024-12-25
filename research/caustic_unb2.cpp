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
    Image four = Image("caustic_unb_4.exr");

    Image image = one * (1003.0) + two * (994.0) + three * (1010.0) + four * (1021.0);
    double total = 1003.0 + 994.0 + 1010.0 + 1021.0;

    image /= total;

    imedit::write_image("caustic_unb_first.exr", image); // 4028
    std::cout << "total first: " << total << std::endl;
}

int main()
{
    std::string path = "/Users/corneria/Desktop/cluster_caustic_unb/images/";
    std::string note_path = "/Users/corneria/Desktop/cluster_caustic_unb/notes/";

    Image fd_image = Image(512, 512);

    int iters = 2048;
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

            Image scene_h00 = Image(path + std::to_string(i) + "_scene_h00.exr");
            Image scene_h01 = Image(path + std::to_string(i) + "_scene_h01.exr");
            Image scene_h10 = Image(path + std::to_string(i) + "_scene_h10.exr");
            Image scene_h11 = Image(path + std::to_string(i) + "_scene_h11.exr");
            Image scene_h20 = Image(path + std::to_string(i) + "_scene_h20.exr");
            Image scene_h21 = Image(path + std::to_string(i) + "_scene_h21.exr");

            if (scene_h00.width() != 512) throw std::exception();
            if (scene_h01.width() != 512) throw std::exception();
            if (scene_h10.width() != 512) throw std::exception();
            if (scene_h11.width() != 512) throw std::exception();
            if (scene_h20.width() != 512) throw std::exception();
            if (scene_h21.width() != 512) throw std::exception();

            Image chg = (scene_h01 - scene_h00) / (2.0*base_h);
            Image dif = (scene_h21 - scene_h20) / (2.0*hp1) - (scene_h11 - scene_h10) / (2.0*h);
            Image eval = chg + dif / pmf;

            if (count == 10)
                imedit::write_image("testing.exr", eval);

            fd_image = fd_image + (eval - fd_image) * (1.0 / double(count+1));

            if (n == 4) {
                Image tmp = (scene_h21 - scene_h20) / (2.0*hp1);
                imedit::write_image("ind/image_" + std::to_string(count)+".exr", tmp);
            }

            count++;
        } catch(...) { std::cout << " iter failed"; }

        std::cout << std::endl;
    }

    std::cout << "FINAL COUNT: " << count << std::endl;

    imedit::write_image("caustic_unb_central.exr", fd_image);
}
