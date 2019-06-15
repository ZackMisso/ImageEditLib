#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

using namespace std;
using namespace imedit;

void noise_one()
{
    int res = 4096;
    std::string extension = ".png";
    double turb_period = double(res) / 64.0;
    double noise_period = double(res) / 16.0;
    double sharp_weight = 0.3;
    int num_tiles = 45;

    std::cout << "creating turb image" << std::endl;
    Image<double> turb_image = Image<double>(res, res, 3);
    turbulence_image_xy(turb_image, turb_period);
    turb_image.write("turb"+extension);
    remap_range_lin(turb_image);

    std::cout << "creating noise image" << std::endl;
    Image<double> noise_image = Image<double>(res, res, 3);
    noise_image_xy(noise_image, noise_period);
    im_abs(noise_image);

    std::cout << "creating sharp image" << std::endl;
    Image<double> sharp_image = Image<double>(res, res, 3);
    sharpen3x3(turb_image, sharp_image);
    remap_avg(sharp_image, 0.0);
    sharp_image = sharp_image * sharp_weight + turb_image;
    sharp_image.write("sharp"+extension);

    std::cout << "creating green image" << std::endl;
    Image<double> green_image = Image<double>(res, res, 3);
    green_image.setPixels(0.03, 0.1, 0.5);
    green_image *= 2.12;

    std::cout << "finishing up noise image" << std::endl;
    noise_image = noise_image * green_image * 2.0;
    noise_image.write("noise"+extension);

    std::cout << "creating sharp green image" << std::endl;
    green_image.write("green"+extension);
    green_image = green_image * sharp_image;
    double max = green_image.max();
    Image<double> max_image = Image<double>(res, res, 3);
    max_image.setPixels(max, max, max);
    green_image = max_image - green_image;
    green_image.write("green_sharp"+extension);

    std::cout << "creating manhattan distance tile image" << std::endl;
    Image<double> man_tile_image = Image<double>(res, res, 3);
    std::vector<std::pair<int, int> > pts = std::vector<std::pair<int, int> >();
    pcg32 rng = pcg32(0x135fa, 0xdbc45);
    for (int i = 0; i < num_tiles; ++i)
    {
        int one = (int)(rng.nextDouble() * res);
        int two = (int)(rng.nextDouble() * res);

        pts.push_back(std::pair<int, int>(one, two));
    }
    manhattan_dist_from_points_image(man_tile_image, pts);
    man_tile_image /= man_tile_image.max();
    man_tile_image.write("man_tile"+extension);

    std::cout << "creating manhattan tiled image" << std::endl;
    Image<double> tile_image = Image<double>(res, res, 3);
    std::vector<Pixel> pixels = std::vector<Pixel>();
    for (int i = 0; i < num_tiles; ++i)
    {
        Pixel pixel;
        pixel.r = rng.nextDouble();
        pixel.g = rng.nextDouble();
        pixel.b = rng.nextDouble();

        pixels.push_back(pixel);
    }
    manhattan_tiled_image(tile_image, pts, pixels);
    tile_image.write("tiled"+extension);

    std::cout << "creating euclidean distance tile image" << std::endl;
    Image<double> euc_dist_image = Image<double>(res, res, 3);
    std::vector<std::pair<double, double> > euc_pts = std::vector<std::pair<double, double> >();
    // pcg32 rng = pcg32(0x135fa, 0xdbc45);
    for (int i = 0; i < num_tiles; ++i)
    {
        double one = (double)(rng.nextDouble() * double(res));
        double two = (double)(rng.nextDouble() * double(res));

        euc_pts.push_back(std::pair<double, double>(one, two));
    }
    euclidean_dist_from_points_image(euc_dist_image, euc_pts);
    euc_dist_image /= euc_dist_image.max();
    euc_dist_image.write("euc_dist"+extension);

    std::cout << "creating euclidean tiled image" << std::endl;
    Image<double> euc_tile_image = Image<double>(res, res, 3);
    pixels.clear();
    for (int i = 0; i < num_tiles; ++i)
    {
        Pixel pixel;
        pixel.r = rng.nextDouble();
        pixel.g = rng.nextDouble();
        pixel.b = rng.nextDouble();

        pixels.push_back(pixel);
    }
    euclidean_tiled_image(euc_tile_image, euc_pts, pixels);
    euc_tile_image.write("euc_tiled"+extension);
}

void create_tiled_image(const std::string& name,
                        const std::vector<Pixel>& colors,
                        unsigned long seed_one,
                        unsigned long seed_two,
                        int res,
                        bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image<double> image = Image<double>(res, res, 3);

    if (is_manhatan)
    {
        std::vector<std::pair<int, int> > pts = std::vector<std::pair<int, int> >();
        for (int i = 0; i < colors.size(); ++i)
        {
            int one = (int)(rng.nextDouble() * res);
            int two = (int)(rng.nextDouble() * res);

            pts.push_back(std::pair<int, int>(one, two));
        }
        manhattan_tiled_image(image, pts, colors);
    }
    else
    {
        std::vector<std::pair<double, double> > euc_pts = std::vector<std::pair<double, double> >();
        for (int i = 0; i < colors.size(); ++i)
        {
            double one = (double)(rng.nextDouble() * double(res));
            double two = (double)(rng.nextDouble() * double(res));

            euc_pts.push_back(std::pair<double, double>(one, two));
        }
        euclidean_tiled_image(image, euc_pts, colors);
    }

    image.write(name);
}

void create_pokedot_image(std::string name,
                          Pixel base,
                          double min_dist,
                          double max_dist,
                          unsigned long seed_one,
                          unsigned long seed_two,
                          int dots,
                          int res,
                          bool is_manhatan)
{
    pcg32 rng = pcg32(seed_one, seed_two);

    Image<double> image = Image<double>(res, res, 3);
    image.setPixels(0.0, 0.0, 0.0);

    if (is_manhatan)
    {
        for (int i = 0; i < dots; ++i)
        {
            int dist = int(rng.nextDouble() * (max_dist - min_dist) + min_dist);
            int xpos = int(rng.nextDouble() * double(res));
            int ypos = int(rng.nextDouble() * double(res));

            splat_manhattan(image, base, xpos, ypos, dist);
        }
    }
    else
    {
        for (int i = 0; i < dots; ++i)
        {
            double dist = rng.nextDouble() * (max_dist - min_dist) + min_dist;
            double xpos = rng.nextDouble() * double(res);
            double ypos = rng.nextDouble() * double(res);

            splat_euclidean(image, base, xpos, ypos, dist);
        }
    }

    image.write(name);
}

void generate_pawn_scene_board(const std::vector<Pixel>& pixels,
                               Pixel dot_base,
                               const std::string base_name,
                               unsigned long seed_one,
                               unsigned long seed_two,
                               double min_dot_dist,
                               double max_dot_dist,
                               int res,
                               int dots,
                               bool manhattan)
{
    pcg32 global_rng = pcg32(seed_one, seed_two);

    for (int i = 0; i < 8; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            std::cout << "creating texture for tile: (" << j << "," << i << ")" << std::endl;

            std::string name = base_name + "_" + to_string(i) + "_" + to_string(j) + ".png";

            if ((i+j)%2 == 0)
            {
                // create tiled image
                create_tiled_image(name,
                                   pixels,
                                   global_rng.nextUInt(),
                                   global_rng.nextUInt(),
                                   res,
                                   manhattan);
            }
            else
            {
                // create dotted image
                create_pokedot_image(name,
                                     dot_base,
                                     min_dot_dist,
                                     max_dot_dist,
                                     global_rng.nextUInt(),
                                     global_rng.nextUInt(),
                                     dots,
                                     res,
                                     manhattan);
            }
        }
    }
}

int main()
{
    noise_one();

    // todo - create main script
    // todo - create interpolation of colors in hsl space

    return 0;
}
