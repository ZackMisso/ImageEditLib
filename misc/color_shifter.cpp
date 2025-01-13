// this is an experimental tool to take a drawing convert it into
// HSL space, then create a grid of alternate color shifts by 
// taking one color and rotating its hue.
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

void parse_image_to_use(Imaged& image) {
    // TODO
}

void hue_shift
(
    Imaged& image,
    double from_hue,
    double to_hue,
    double threshold = 0.04
) {
    // shift all pixels whose hue lie within some threshold
    // TODO
}

void hue_shift_mask_in_place
(
    Imaged& image,
    double from_hue,
    double to_hue,
    double threshold = 0.04
) {
    // places a mask over the og image for debugging to indicate where
    // the hue should shift
    // TODO
}

void hue_shift_mask
(
    const Imaged& image,
    Imaged& mask,
    double from_hue,
    double to_hue,
    double threshold = 0.04
) {
    // creates a mask for debugging to indicate where the hue shifts
    // TODO
}

int main(int argc, char *argv[])
{
    std::cout << "starting color_shifter v:0.0.1" << std::endl;

    Imaged og_image;
    parse_image_to_use(og_image);

    // TODO
}

void game_proj() {
    // TODO last
}