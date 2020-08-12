#include <iostream>
#include <imedit/image.h>
#include <fstream>

int main(int argc, char* argv[])
{
    std::string path = std::string(argv[1]);
    std::string old_ext = std::string(argv[2]);
    std::string new_ext = std::string(argv[3]);

    imedit::Image img = imedit::Image(path + old_ext);
    img.write(path + new_ext);
}
