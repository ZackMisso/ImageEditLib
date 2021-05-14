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
    std::string path = "/Users/corneria/Desktop/cluster_caustic_unb/images/";
    std::string note_path = "/Users/corneria/Desktop/cluster_caustic_unb/notes/";

    imedit::Image fd_image = imedit::Image(1024, 1024);

    // combine();
    //
    // return 0;

    // a list of failures which have failed due to timing out.
    std::vector<int> failures = std::vector<int>();
    // failures.push_back(771);
    // failures.push_back(774);
    // failures.push_back(746);
    // failures.push_back(777);
    // failures.push_back(819);
    // failures.push_back(1017);
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

    int iters = 2048;
    int count = 0;

    for (int i = 0; i < iters; ++i)
    {
        std::cout << "i: " << i;
        //
        // bool skip = false;
        //
        // for (int j = 0; j < failures.size(); ++j)
        // {
        //     if (i == failures[j]) skip = true;
        // }
        //
        // if (!skip)
        // {

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

            // std::cout << "what" << std::endl;

            imedit::Image scene = imedit::Image(path + "scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << scene.width() << std::endl;
            imedit::Image basescene = imedit::Image(path + "base_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << basescene.width() << std::endl;
            imedit::Image hscene = imedit::Image(path + "h_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what hp1 " << hscene.width() << std::endl;
            imedit::Image hp1scene = imedit::Image(path + "hp1_scene_" + std::to_string(i) + ".exr");
            // std::cout << "what " << hp1scene.width() << std::endl;

            if (scene.width() != 1024) throw std::exception();
            if (basescene.width() != 1024) throw std::exception();
            if (hscene.width() != 1024) throw std::exception();
            if (hp1scene.width() != 1024) throw std::exception();

            // std::cout << hp1scene.height() << std::endl;
            // std::cout << scene.height() << std::endl;

            // std::cout << "huh: " << base_h << std::endl;

            imedit::Image chg = (basescene - scene) / base_h;
            // std::cout << "quiff: " << h << " as: " << hp1 << std::endl;
            imedit::Image dif = (hp1scene - scene) / hp1 - (hscene - scene) / h;
            // std::cout << "dif: " << pmf << std::endl;
            imedit::Image eval = dif;

            eval.write("ind/"+std::to_string(i) + "_" + std::to_string(int(n))+ ".exr");

            // std::cout << "quack" << std::endl;

            // std::cout << base_h << std::endl;
            // std::cout << n << std::endl;
            // std::cout << h << std::endl;
            // std::cout << hp1 << std::endl;
            // std::cout << pmf << std::endl;
            //
            // return 0;

            fd_image = fd_image + (eval - fd_image) * (1.0 / double(count+1));

            count++;
        } catch(std::exception e) { std::cout << " iter failed"; }

        std::cout << std::endl;
    }

    std::cout << "FINAL COUNT: " << count << std::endl;

    fd_image.write("caustic_unb_fuck.exr");
}
