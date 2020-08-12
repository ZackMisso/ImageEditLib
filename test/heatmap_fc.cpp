#include <iostream>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <imedit/im_color_maps.h>
#include <fstream>
#include <vector>
#include <dirent.h>
#include <sys/types.h>
#include <cstring>
#include <stdio.h>

int main(int argc, char* argv[])
{
    std::string full_path_to_data = std::string(argv[1]);
    std::string full_path_to_write = std::string(argv[2]);

    std::vector<std::string> data_files = std::vector<std::string>();

    DIR* dir_file = opendir(full_path_to_data.c_str());

    if (dir_file)
    {
        struct dirent* h_file;
        while ((h_file = readdir(dir_file)) != NULL)
        {
            // if (!strcmp(h_file->d_name, ".")) continue;
            // if (!strcmp(h_file->d_name, "..")) continue;
            // if (gIgnoreHidden && (h_file->d_name[0] == '.')) continue;
            std::string str = std::string(h_file->d_name);
            int len = str.length();

            if (len > 4 && str[len-1] == 't'&& str[len-2] == 'x' && str[len-3] == 't' && str[len-4] == '.')
            {
                data_files.push_back(h_file->d_name);
            }
        }
        closedir( dir_file );
    }

    std::vector<imedit::Image> images = std::vector<imedit::Image>();
    std::vector<std::string> outputs = std::vector<std::string>();

    for (int i = 0; i < data_files.size(); ++i)
    {
        std::string path_to_data = full_path_to_data + data_files[i];

        std::ifstream file_stream;
        // std::cout << path_to_data << std::endl;
        file_stream.open(path_to_data);

        std::string val;
        int grid_size;

        file_stream >> val;
        grid_size = std::stoi(val.c_str());
        // std::cout << "gridsize: " << grid_size << std::endl;

        imedit::Image img = imedit::Image(grid_size, grid_size);

        for (int i = 0; i < grid_size; ++i)
        {
            for (int j = 0; j < grid_size; ++j)
            {
                float val;
                file_stream >> val;
                img(j, i) = imedit::Pixel(val);
            }
        }

        images.push_back(img);

        outputs.push_back(full_path_to_write + data_files[i] + ".png");
        img.write(full_path_to_write + "non_fc_" + data_files[i] + ".png");
    }

    // TODO: apply the false color stuff now
    float min_val = 100000000000000.0;
    float max_val = 0.0;

    std::vector<imedit::Pixel> color_map = std::vector<imedit::Pixel>();
    imedit::color_map_inferno(color_map);

    for (int i = 0; i < images.size(); ++i)
    {
        if (images[i].min() < min_val) min_val = images[i].min();
        if (images[i].max() > max_val) max_val = images[i].max();

        imedit::Image image = imedit::Image(images[i].width(), images[i].height());

        imedit::falseColorProxy(images[i], image, min_val, max_val);

        for (int k = 0; k < image.height(); ++k)
        {
            for (int j = 0; j < image.width(); ++j)
            {
                image(j, k) = imedit::color_from_proxy(color_map, image(j, k).r);
            }
        }

        image.write(outputs[i]);
    }
}
