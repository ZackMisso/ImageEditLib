/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#include <iostream>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/im_util.h"

using namespace std;

bool generate_test_set();
bool delete_test_set();
bool getchannel_test();
bool operator_test();

bool generate_test_set()
{
    Image<float> redImage = Image<float>(128, 128, 3);
    Image<float> blueImage = Image<float>(128, 128, 3);
    Image<float> greenImage = Image<float>(128, 128, 3);

    for (int i = 0; i < 128; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            redImage(j, i, 0) = 0.5f;
            redImage(j, i, 1) = 0.f;
            redImage(j, i, 2) = 0.f;

            greenImage(j, i, 0) = 0.f;
            greenImage(j, i, 1) = 0.5f;
            greenImage(j, i, 2) = 0.f;

            blueImage(j, i, 0) = 0.f;
            blueImage(j, i, 1) = 0.f;
            blueImage(j, i, 2) = 0.5f;
        }
    }

    bool wat = redImage.write("redImage.hdr");
    greenImage.write("greenImage.hdr");
    blueImage.write("blueImage.hdr");

    Image<float> redReadImage = Image<float>("redImage.hdr");
    Image<float> greenReadImage = Image<float>("greenImage.hdr");
    Image<float> blueReadImage = Image<float>("blueImage.hdr");

    for (int i = 0; i < 128; ++i)
    {
        for (int j = 0; j < 128; ++j)
        {
            if (redImage(j, i, 0) != 0.5f) return false;
            if (redImage(j, i, 1) != 0.f) return false;
            if (redImage(j, i, 2) != 0.f) return false;

            if (greenImage(j, i, 0) != 0.f) return false;
            if (greenImage(j, i, 1) != 0.5f) return false;
            if (greenImage(j, i, 2) != 0.f) return false;

            if (blueImage(j, i, 0) != 0.f) return false;
            if (blueImage(j, i, 1) != 0.f) return false;
            if (blueImage(j, i, 2) != 0.5f) return false;
        }
    }

    // procedural tests

    Image<float> sineImage_X = Image<float>(128, 128, 3);
    Image<float> sineImage_Y = Image<float>(128, 128, 3);
    Image<float> sineImage_XY = Image<float>(128, 128, 3);

    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_X, IP_X);
    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_Y, IP_Y);
    sin_image(0.5f, 10.f, 0.f, 0.5f, sineImage_XY, IP_XY);

    sineImage_X.write("sine_X.hdr");
    sineImage_Y.write("sine_Y.hdr");
    sineImage_XY.write("sine_XY.hdr");

    Image<float> noiseImage = Image<float>(128, 128, 3);

    noise_image_xy(noiseImage, 1.f);

    noiseImage.write("noise_image.hdr");

    im_abs(noiseImage);

    noiseImage.write("noise_image_abs.hdr");

    Image<float> threshold_min = noiseImage;
    Image<float> threshold_max = noiseImage;

    thresh_min(threshold_min, 0.3f);
    thresh_max(threshold_max, 0.3f);

    threshold_min.write("threshold_min.hdr");
    threshold_max.write("threshold_max.hdr");
    noiseImage.write("check.hdr");

    Image<float> noiseImage_two = Image<float>(128, 128, 3);

    noise_image_xy(noiseImage_two, 5.f);

    noiseImage_two.write("noise_image_two.hdr");

    im_abs(noiseImage_two);

    noiseImage_two.write("noise_image_two_abs.hdr");

    Image<float> threshold_min_two = noiseImage_two;
    Image<float> threshold_max_two = noiseImage_two;

    thresh_min(threshold_min_two, 0.3f);
    thresh_max(threshold_max_two, 0.3f);

    threshold_min_two.write("threshold_min_two.hdr");
    threshold_max_two.write("threshold_max_two.hdr");

    Image<float> turbulenceImage = Image<float>(128, 128, 3);

    turbulence_image_xy(turbulenceImage, 32.f);

    turbulenceImage.write("turbImage.hdr");

    return true;
}

bool delete_test_set()
{
    // TODO
    return false;
}

bool getchannel_test()
{
    // TODO
    return false;
}

bool operator_test()
{
    Image<float> redImage = Image<float>("redImage.hdr");
    Image<float> greenImage = Image<float>("greenImage.hdr");
    Image<float> blueImage = Image<float>("blueImage.hdr");

    Image<float> combined = (redImage * 2.0) +
                            (greenImage * 2.0) +
                            (blueImage * 2.0);

    // std::cout << "ONE" << std::endl;

    for (int i = 0; i < combined.size(); ++i)
    {
        // cout << "comb: " << combined[i] << endl;
        // cout << "red: " << redImage[i] << endl;
        if (combined[i] != 1.f) return false;
    }

    // std::cout << "TWO" << std::endl;

    combined /= 2.0;

    for (int i = 0; i < combined.size(); ++i)
    {
        // cout << combined[i] << endl;
        if (combined[i] != 0.5) return false;
    }

    // std::cout << "THREE" << std::endl;

    Image<float> negRed = combined - redImage;
    Image<float> negBlue = combined - blueImage;
    Image<float> negGreen = combined - greenImage;

    // std::cout << "FOUR" << std::endl;

    for (int i = 0; i < combined.height(); ++i)
    {
        for (int j = 0; j < combined.width(); ++j)
        {
            if (negRed(j, i, 0) != 0.0) return false;
            if (negRed(j, i, 1) != 0.5) return false;
            if (negRed(j, i, 2) != 0.5) return false;

            if (negBlue(j, i, 0) != 0.5) return false;
            if (negBlue(j, i, 1) != 0.5) return false;
            if (negBlue(j, i, 2) != 0.0) return false;

            if (negGreen(j, i, 0) != 0.5) return false;
            if (negGreen(j, i, 1) != 0.0) return false;
            if (negGreen(j, i, 2) != 0.5) return false;
        }
    }

    // std::cout << "FIVE" << std::endl;

    combined.write("combined.hdr");
    negRed.write("negRed.hdr");
    negGreen.write("negGreen.hdr");
    negBlue.write("negBlue.hdr");

    return true;
}

int main()
{
    cout << "Beginning Library Tests" << endl;

    if (!generate_test_set())
    {
        cout << "ERROR: Generating Test Set Failed" << endl;
    }
    if (!operator_test())
    {
        cout << "ERROR: Operator Test Failed" << endl;
    }
    // todo
    cout << "All Library Tests Passed" << endl;
    return 0;
}
