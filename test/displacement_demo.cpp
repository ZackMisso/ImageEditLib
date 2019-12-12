#include <iostream>
#include <stdio.h>
#include "imedit/image.h"
#include "imedit/procedural.h"
#include "imedit/filter.h"
#include "imedit/im_util.h"
#include "pcg/pcg32.h"

typedef std::pair<double, double> Pff;
typedef imedit::Pixel Pix;

#define PI 3.14159265

int mask_timer = 50;

struct Location_Time
{
    Location_Time(double xpos, double ypos, int time)
        : xpos(xpos), ypos(ypos), time(time) { }

    double xpos;
    double ypos;
    int time;
};

// TODO: make this a library function
void copy_image_if_pos_with_mask(imedit::Image& image, imedit::Image& mask, imedit::Image& copy, double cx, double cy)
{
    int initial_x = floor(cx - double(copy.width()) / 2.0);
    int initial_y = floor(cy - double(copy.height()) / 2.0);

    for (int x = initial_x; x < initial_x + copy.width(); ++x)
    {
        for (int y = initial_y; y < initial_y + copy.height(); ++y)
        {
            if (x >= 0 && x < image.width() &&
                y >= 0 && y < image.height())
            {
                if (x - initial_x < 0 || x - initial_x >= copy.width()) std::cout << "FUCK" << std::endl;
                if (y - initial_y < 0 || y - initial_y >= copy.height()) std::cout << "FUCK" << std::endl;
                if (copy(x - initial_x, y - initial_y, 0) > 0.0)
                {
                    image(x, y, 0) = copy(x - initial_x, y - initial_y, 0);
                    mask(x, y, 0) = mask_timer;
                }
                if (copy(x - initial_x, y - initial_y, 1) > 0.0)
                {
                    image(x, y, 1) = copy(x - initial_x, y - initial_y, 1);
                    mask(x, y, 1) = mask_timer;
                }
                if (copy(x - initial_x, y - initial_y, 2) > 0.0)
                {
                    image(x, y, 2) = copy(x - initial_x, y - initial_y, 2);
                    mask(x, y, 2) = mask_timer;
                }
            }
        }
    }
}

// TODO: make this a library function
void check_mask(imedit::Image& image, imedit::Image& mask)
{
    mask -= 1.f;

    imedit::apply_mask(image, mask, 40.f, 0.985f);
}

int main(int argc, char* argv[])
{
    // initialize global parameters
    pcg32 rng = pcg32(0x4235, 0xac42);
    int res = 1024;
    int frames = 1000;
    std::string location = "displacement/";

    std::string rm_command = "rm -rf " + location;
    std::string mkdir_command = "mkdir " + location;

    std::string publish_command_1 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/disp_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_1.mp4";
    std::string publish_command_2 = "ffmpeg -r 60 -f image2 -s 1024x1024 -i " + location + "/mask_%04d.png -vcodec libx264 -crf 25  -pix_fmt yuv420p " + location + "/test_1_mask.mp4";

    system(rm_command.c_str());
    system(mkdir_command.c_str());

    imedit::Image word_image_initial = imedit::Image("../data/ernie_ball.png");
    ~word_image_initial;

    imedit::Image word_image = imedit::Image(word_image_initial.width()*2, word_image_initial.height()*2, 3);

    for (int i = 0; i < word_image_initial.height(); ++i)
    {
        for (int j = 0; j < word_image_initial.width(); ++j)
        {
            word_image(2*j, 2*i, 0) = word_image_initial(j, i, 0);
            word_image(2*j, 2*i, 1) = word_image_initial(j, i, 1);
            word_image(2*j, 2*i, 2) = word_image_initial(j, i, 2);

            word_image(2*j+1, 2*i, 0) = word_image_initial(j, i, 0);
            word_image(2*j+1, 2*i, 1) = word_image_initial(j, i, 1);
            word_image(2*j+1, 2*i, 2) = word_image_initial(j, i, 2);

            word_image(2*j, 2*i+1, 0) = word_image_initial(j, i, 0);
            word_image(2*j, 2*i+1, 1) = word_image_initial(j, i, 1);
            word_image(2*j, 2*i+1, 2) = word_image_initial(j, i, 2);

            word_image(2*j+1, 2*i+1, 0) = word_image_initial(j, i, 0);
            word_image(2*j+1, 2*i+1, 1) = word_image_initial(j, i, 1);
            word_image(2*j+1, 2*i+1, 2) = word_image_initial(j, i, 2);
        }
    }

    word_image *= 2.f;
    imedit::clamp_im(word_image);

    word_image.write("initial_word.png");

    std::vector<Location_Time> time_stamps_one = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_two = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_three = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_four = std::vector<Location_Time>();
    std::vector<Location_Time> time_stamps_five = std::vector<Location_Time>();

    float start_x = float(res) / 2.0 + word_image.width() / 2.0;
    float start_y = 2.75 * float(res) / 4.0;
    int start_t = 0;

    float end_x = float(res) / 2.0 - word_image.width() / 3.0;
    float end_y = 2.75 * float(res) / 4.0;
    int end_t = word_image.width() / 3;

    // std::cout << "end_T: " << end_t << std::endl;
    // std::cout << "last_T: " << last_t << std::endl;

    int last_t = 4 * word_image.width() / 2;

    end_t /= 2;

    int time_diff = 1002;
    time_diff = word_image.width();

    Pff last_one = Pff(cos(180.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(180.0 * PI / 180.0) * double(time_diff));
    Pff last_two = Pff(cos(157.5 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(157.5 * PI / 180.0) * double(time_diff));
    Pff last_three = Pff(cos(135.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(135.0 * PI / 180.0) * double(time_diff));
    Pff last_four = Pff(cos(112.5 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(112.5 * PI / 180.0) * double(time_diff));
    Pff last_five = Pff(cos(90.0 * PI / 180.0) * double(time_diff) + end_x, end_y - sin(90.0 * PI / 180.0) * double(time_diff));

    time_stamps_one.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_one.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_one.push_back(Location_Time(last_one.first, last_one.second, last_t));

    time_stamps_two.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_two.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_two.push_back(Location_Time(last_two.first, last_two.second, last_t));

    time_stamps_three.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_three.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_three.push_back(Location_Time(last_three.first, last_three.second, last_t));

    time_stamps_four.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_four.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_four.push_back(Location_Time(last_four.first, last_four.second, last_t));

    time_stamps_five.push_back(Location_Time(start_x, start_y, start_t));
    time_stamps_five.push_back(Location_Time(end_x, end_y, end_t));
    time_stamps_five.push_back(Location_Time(last_five.first, last_five.second, last_t));

    imedit::Image current_image = imedit::Image(res, res, 3);
    imedit::Image current_mask = imedit::Image(res, res, 3);

    int time_stamp = 1;

    for (int i = 0; i < current_mask.size(); ++i)
    {
        current_mask[i] = 1.0;
    }

    // TODO: this currently only works if all of the time stamps are the same
    for (int i = 0; i < frames; ++i)
    {
        std::cout << "creating frame: " << (i+1) << " out of: " << frames << " frames" << std::endl;

        if (i > time_stamps_one[time_stamp].time && time_stamp < time_stamps_one.size())
        {
            time_stamp++;
        }

        if (time_stamp >= time_stamps_one.size())
        {
            // ONE
            {
                Location_Time stamp = time_stamps_one[time_stamps_one.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // TWO
            {
                Location_Time stamp = time_stamps_two[time_stamps_two.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // THREE
            {
                Location_Time stamp = time_stamps_three[time_stamps_three.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // FOUR
            {
                Location_Time stamp = time_stamps_four[time_stamps_four.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }

            // FIVE
            {
                Location_Time stamp = time_stamps_five[time_stamps_five.size()-1];

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, stamp.xpos, stamp.ypos);
            }
        }
        else
        {
            // ONE
            {
                Location_Time stamp_one = time_stamps_one[time_stamp-1];
                Location_Time stamp_two = time_stamps_one[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // TWO
            {
                Location_Time stamp_one = time_stamps_two[time_stamp-1];
                Location_Time stamp_two = time_stamps_two[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // THREE
            {
                Location_Time stamp_one = time_stamps_three[time_stamp-1];
                Location_Time stamp_two = time_stamps_three[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // FOUR
            {
                Location_Time stamp_one = time_stamps_four[time_stamp-1];
                Location_Time stamp_two = time_stamps_four[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }

            // FIVE
            {
                Location_Time stamp_one = time_stamps_five[time_stamp-1];
                Location_Time stamp_two = time_stamps_five[time_stamp];

                double t = (stamp_two.time == stamp_one.time) ? 0.0 : double(i - stamp_one.time) / double(stamp_two.time - stamp_one.time);

                double xpos = stamp_two.xpos * t + (1.0 - t) * stamp_one.xpos;
                double ypos = stamp_two.ypos * t + (1.0 - t) * stamp_one.ypos;

                copy_image_if_pos_with_mask(current_image, current_mask, word_image, xpos, ypos);
            }
        }

        check_mask(current_image, current_mask);

        current_image = mirror_along_x_axis(current_image, current_image.width() / 2);
        current_mask = mirror_along_x_axis(current_mask, current_mask.width() / 2);

        current_image = mirror_along_y_axis(current_image, 4 * current_image.height() / 5);
        current_mask = mirror_along_y_axis(current_mask, 4 * current_mask.height() / 5);

        char str[5];
        snprintf(str, 5, "%04d", i);

        imedit::Image normalized_mask = current_mask / float(mask_timer);

        current_image.write(location + "disp_" + std::string(str) + ".png");
        normalized_mask.write(location + "mask_" + std::string(str) + ".png");

        // imedit::Image backed_image = current_image * background;
        // imedit::Image backed_mask = normalized_mask * background;
        //
        // backed_image.write("adrien/backed_word_" + std::string(str) + ".png");
        // backed_mask.write("adrien/backed_mask_" + std::string(str) + ".png");
    }

    // TODO: how to organize this library of operations - should images be recomputed everytime or stored
    // as functions?

    system(publish_command_1.c_str());
    system(publish_command_2.c_str());

    // imedit::Image mirrored = mirror_along_x_axis(word_image, word_image.width()/2);
    // mirrored.write("initial_mirrored.png");

    return 0;
}
