#include <iostream>
#include <imedit/image.h>
#include <pcg32.h>
#include <fstream>
#include <string>
#include <dirent.h>
// namespace fs = std::experimental::filesystem;

void get_bounding_box(const imedit::Image& mask,
                      int& min_x,
                      int& min_y,
                      int& max_x,
                      int& max_y)
{
    bool found_first = false;
    for (int i = 0; i < mask.height(); i++)
    {
        for (int j = 0; j < mask.width(); ++j)
        {
            if (mask(j, i).r > 1e-4)
            {
                if (!found_first)
                {
                    min_x = j;
                    min_y = i;
                    max_x = j;
                    max_y = i;
                    found_first = true;
                }
                else
                {
                    min_x = std::min(min_x, j);
                    min_y = std::min(min_y, i);
                    max_x = std::max(max_x, j);
                    max_y = std::max(max_y, i);
                }
            }
        }
    }
}

std::string format_name(int index)
{
    std::string str = std::to_string(index);
    while (str.length() != 6)
    {
        str = "0" + str;
    }
    return str;
}

int main(int argc, char* argv[])
{
    std::string path = std::string(argv[2]);
    std::vector<std::string> composite_images_paths = std::vector<std::string>();

    // std::cout << "who" << std::endl;
    if (argc != 5) return 0;
    // std::cout << "what" << std::endl;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir(path.c_str())) != nullptr)
    {
        /* print all the files and directories within directory */
        while (((ent = readdir (dir))) != nullptr)
        {
            composite_images_paths.push_back(std::string(ent->d_name));
        }
        closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return EXIT_FAILURE;
    }

    std::string exp_path = std::string(argv[1]);
    int num_images = std::stoi(argv[3]);
    std::string res_path = std::string(argv[4]);
    pcg32 rng = pcg32(0x32412, 0xac12);

    for (int i = 0; i < num_images; ++i)
    {
        // std::string mkdir_str = "mkdir " + res_path;
        // system(mkdir_str.c_str());
        // std::cout << exp_path + "img_" + format_name(i) + ".exr" << std::endl;
        imedit::Image img = imedit::Image(exp_path + "img_" + format_name(i) + ".exr");
        imedit::Image mask = imedit::Image(exp_path + "mask_" + format_name(i) + ".exr");

        int min_x, min_y, max_x, max_y;
        get_bounding_box(mask, min_x, min_y, max_x, max_y);

        std::ofstream file;
        file.open(exp_path + "bbox_" + format_name(i) + ".txt");
        file << min_x << std::endl;
        file << min_y << std::endl;
        file << max_x << std::endl;
        file << max_y << std::endl;
        file.close();

        imedit::Image alph_image;
        // std::cout << "reading alpha" << std::endl;
        alph_image.read_with_mask(exp_path + "mask_" + format_name(i) + ".exr");
        // std::cout << "post reading alpha" << std::endl;

        imedit::Image final_image = img;

        uint32_t image_index = rng.nextUInt(composite_images_paths.size());

        imedit::Image composite_image = imedit::Image(path + composite_images_paths[image_index]);

        for (int i = 0; i < img.height(); ++i)
        {
            for (int j = 0; j < img.width(); ++j)
            {
                // std::cout << "alpha_val: " << alph_image.mask_val(j, i) << std::endl;
                img(j, i) = img(j, i) * alph_image.mask_val(j, i) + composite_image(j, i) * (1.f - alph_image.mask_val(j, i));
            }
        }

        img.write(res_path + "img_" + format_name(i) + ".png");
        mask.write(res_path + "mask_" + format_name(i) + ".png");
    }
}
