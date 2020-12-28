#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        std::cout << "ERROR: this program requires 5 inputs" << std::endl;
        return 0;
    }

    std::string im_directory = std::string(argv[1]);
    std::string gt_image_name = std::string(argv[2]);
    int start = std::stoi(argv[3]);
    int end = std::stoi(argv[4]); // end n, n+1 still has to exist

    imedit::Image gt_image = imedit::Image(gt_image_name);

    int width = gt_image.width();
    int height = gt_image.height();

    // create the difference images
    for (int i = start; i < end; ++i)
    {
        imedit::Image inp1 = imedit::Image(im_directory + std::to_string(i+1) + ".exr");
        imedit::Image in = imedit::Image(im_directory + std::to_string(i) + ".exr");

        imedit::Image diff = inp1-in;
        imedit::im_abs(diff);

        diff.write(im_directory + std::to_string(i) + "_" + std::to_string(i+1) + ".exr");
    }

    // create actual data
    std::ofstream file;
    file.open(im_directory + "pw_mse.txt");
    file << width << std::endl;
    file << height << std::endl;
    file << start << std::endl;
    file << end << std::endl;

    std::vector<imedit::Image> images = std::vector<imedit::Image>();

    for (int k = start; k <= end; ++k)
    {
        images.push_back(imedit::Image(im_directory + std::to_string(k) + ".exr"));
    }

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            std::vector<double> diffs = std::vector<double>();
            double sum = 0.0;

            for (int k = 0; k < end - start; ++k)
            {
                // take the average of the differences per pixel channel
                double diff = std::abs(images[k+1](j, i, 0) - images[k](j, i, 0)) +
                              std::abs(images[k+1](j, i, 1) - images[k](j, i, 1)) +
                              std::abs(images[k+1](j, i, 2) - images[k](j, i, 2));
                // diff /= 3.0;

                diffs.push_back(diff);
                sum += diff;
            }

            for (int k = 0; k < diffs.size(); ++k)
            {
                file << diffs[k] / sum << " ";
            }

            file << std::endl;
        }
    }

    file.close();

    return 0;
}
