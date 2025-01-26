// this is an experimental tool to take a drawing with a fixed color
// palette, convert it into HSL space, then create a grid of
// alternate ideas by performing simple rotations in hue and
// potentially allow alterations to the saturation and luminance.
// If this tool is useful, I might add it as a function in im_util.h

/// change log
//
// v:0.0.1 - initial implementation

#include <iostream>
#include <fstream>
#include <tgmath.h>
#include <pcg32.h>
#include "imedit/image.h"
#include "imedit/im_util.h"
#include "imedit/procedural.h"
#include "imedit/noise.h"
#include "imedit/im_color_maps.h"
#include "imedit/filter.h"

#define Imaged imedit::RGBImage<double>
#define Pixd imedit::Pixel<double>

struct Palette
{
    Palette() {
        colors = std::vector<Pixd>();
    }

    void convert_to_hsl() {
        for (int i = 0; i < colors.size(); ++i) {
            rgb_to_hsl(colors[i]);
        }
    }

    int closest_palette_index(Pixd color) {
        double min_d = 1.0;
        int min_ind = -1;

        for (int i = 0; i < colors.size(); i++) {
            double d = std::min(std::abs(color.r - colors[i].r),
                                std::min(std::abs(color.r - (colors[i].r+1.0)),
                                         std::abs(color.r - (colors[i].r-1.0))));

            if (d < min_d) {
                min_d = d;
                min_ind = i;
            }
        }

        if (min_ind == -1) std::cout << "WHAT" << std::endl;

        return min_ind;
    }

    void add_color(const Pixd& color) {
        colors.push_back(color);
    }

    void clear() {
        colors.clear();
    }

    std::vector<Pixd> colors;
};

double compare_dist(double one, double two) {
    return std::min(std::abs(one - two),
                    std::min(std::abs(one - (two+1.0)),
                                std::abs(one - (two-1.0))));
}

void parse_image_to_use(Imaged& image) {
    imedit::read_image("../private/firal.png", image);
}

Palette* parse_og_palette(const Imaged& image) {
    Palette* palette = new Palette();
    // TODO: enter colors
    palette->colors.push_back(image(3318,3633));
    palette->colors.push_back(image(3522,3633));
    palette->colors.push_back(image(3789,3633));
    palette->colors.push_back(image(4005,3633));
    palette->colors.push_back(image(4249,3633));
    palette->colors.push_back(image(4516,3633));
    palette->colors.push_back(image(4686,3633));
    palette->colors.push_back(image(4794,3633));

    palette->convert_to_hsl();
    return palette;
}

// this currently performs a hue shift
void palette_shift(Imaged& image, Palette* palette, double dist, double threshold = 0.04) {
    image.convert_to_hsl();

    std::vector<int> index = std::vector<int>(image.width()*image.height());

    // for (int i = 0; i < image.height(); ++i) {
    //     for (int j = 0; j < image.width(); ++j) {
    //         index[i * image.width() + j] = palette->closest_palette_index(image(j,i));
    //     }
    // }

    // for (int k = 0; k < palette->colors.size(); ++k) {
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            // if (compare_dist(image(j,i,0), palette->colors[index[i*image.width()+j]].r) <= threshold) {
                image(j,i,0) += dist;
            // }
        }
    }
    // }
    
    image.convert_to_rgb();
}

int main(int argc, char *argv[])
{
    std::cout << "starting palette_grid v:0.0.1" << std::endl;
    
    // std::string path = "../private/";
    int grid_iterations = 100;

    std::string path = "../private/palette_grid_test/";
    std::string mk = "mkdir " + path;
    system(mk.c_str());

    Imaged og_image;
    parse_image_to_use(og_image);

    // imedit::write_image(path + "pre", og_image);

    Palette* palette = parse_og_palette(og_image);

    // TODO: parse inputs (probably in a future version of this tool)
    // TODO: make directory

    // this is for a demo
    for (int i = 0; i <= 516; ++i) {
        std::cout << "creating palette version: " << i << std::endl;
        Imaged copy = og_image;
        // imedit::write_image(path + "pre_iter_" + std::to_string(i)+".exr", copy);
        double dist = double(i-11) / double(grid_iterations+1);
        while (dist >= 1.0) dist -= 1.0;
        dist = std::max(dist, 0.0);
        std::cout << "using distance: " << dist << std::endl;
        palette_shift(copy, palette, dist, 0.01);
        Imaged half = half_sized_image(copy);
        Imaged fourth = half_sized_image(half);
        // imedit::write_image(path + "iter_" + std::to_string(i)+".exr", copy);
        imedit::write_image(path + "iter_fourth_" + std::to_string(i)+".exr", fourth);
    } 
    
    return 0;
}