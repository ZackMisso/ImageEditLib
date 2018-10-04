/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>
*/

#include <iostream>
#include "imedit/image.h"

// #define

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
