#include <iostream>
#include <imedit/image.h>
#include <fstream>

int main(int argc, char* argv[])
{
    std::string path_to_data = std::string(argv[1]);
    std::string path_to_write = std::string(argv[2]);

    std::ifstream file_stream;
    file_stream.open(path_to_data);

    int grid_size;

    file_stream >> grid_size;

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

    img.write(path_to_write);
}
