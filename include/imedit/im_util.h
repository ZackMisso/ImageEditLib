/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>
*/

#pragma once

#include <imedit/image.h>
#include <vector>

template <typename T>
Image<T>* low_avg_comparison(const std::vector<Image<T>*>& images)
{
    double lowest = images[0]->average();
    Image<T>* least_avg_image = images[0];

    for (int i = 1; i < images.size(); ++i)
    {
        double avg = images[i]->average();

        if (avg < lowest)
        {
            lowest = avg;
            least_avg_image = images[i];
        }
    }

    return least_avg_image;
}
