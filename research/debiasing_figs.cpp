#include <iostream>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <fstream>
#include <sstream>

#define Image imedit::RGBImage<double>
#define Pix imedit::Pixel<double>

void create_fd_image(const Image& fd, Image& image)
{
    for (int i = 0; i < image.height(); ++i)
    {
        for (int j = 0; j < image.width(); ++j)
        {
            double lum = (fd(j, i, 0) * 0.3) + (fd(j, i, 1) * 0.59) + (fd(j, i, 2) * 0.11);

            if (lum < 0.0)
            {
                image(j, i, 2) = -lum;
            }
            else
            {
                image(j, i, 0) = lum;
            }
        }
    }
}

void create_didactic_figs()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/didactic/";

    system("mkdir debiasing_figs_images/didactic/");

    Image gt = Image(path + "gt.exr");
    Image bounce_1 = Image(path + "bounce_1.exr");
    Image bounce_2 = Image(path + "bounce_2.exr");
    Image bounce_3 = Image(path + "bounce_3.exr");
    Image bounce_4 = Image(path + "bounce_4.exr");
    Image bounce_5 = Image(path + "bounce_5.exr");
    Image bounce_6 = Image(path + "bounce_6.exr");
    Image bounce_7 = Image(path + "bounce_7.exr");

    Image delta_1 = bounce_1;
    Image delta_2 = bounce_2 - bounce_1;
    Image delta_3 = bounce_3 - bounce_2;
    Image delta_4 = bounce_4 - bounce_3;
    Image delta_5 = bounce_5 - bounce_4;
    Image delta_6 = bounce_6 - bounce_5;
    Image delta_7 = bounce_7 - bounce_6;

    // TODO: do more manipulations if necessary

    imedit::write_image("debiasing_figs_images/didactic/gt.png", gt);
    imedit::write_image("debiasing_figs_images/didactic/delta_1.png", delta_1);
    imedit::write_image("debiasing_figs_images/didactic/delta_2.png", delta_2);
    imedit::write_image("debiasing_figs_images/didactic/delta_3.png", delta_3);
    imedit::write_image("debiasing_figs_images/didactic/delta_4.png", delta_4);
    imedit::write_image("debiasing_figs_images/didactic/delta_5.png", delta_5);
    imedit::write_image("debiasing_figs_images/didactic/delta_6.png", delta_6);
    imedit::write_image("debiasing_figs_images/didactic/delta_7.png", delta_7);
}

void create_smoke_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/smoke/";

    system("mkdir debiasing_figs_images/smoke/");

    Image c3_gt = Image(path + "c3_gt.exr");
    Image c8_gt = Image(path + "c8_gt.exr");
    Image c3_rat = Image(path + "c3_rat.exr");
    Image c3_tay = Image(path + "c3_tay.exr");
    Image c8_rat = Image(path + "c8_rat.exr");
    Image c8_tay = Image(path + "c8_tay.exr");

    int width_cut = 9*8;
    int height_cut = 11*8;

    Image c3_gt_crop = Image(c3_gt.width() - width_cut, c3_gt.height() - height_cut);
    Image c3_rat_crop = Image(c3_rat.width() - width_cut, c3_rat.height() - height_cut);
    Image c3_tay_crop = Image(c3_tay.width() - width_cut, c3_tay.height() - height_cut);
    Image c8_gt_crop = Image(c8_gt.width() - width_cut, c8_gt.height() - height_cut);
    Image c8_rat_crop = Image(c8_rat.width() - width_cut, c8_rat.height() - height_cut);
    Image c8_tay_crop = Image(c8_tay.width() - width_cut, c8_tay.height() - height_cut);

    int x_off = width_cut / 2;
    int y_off = height_cut / 2;

    for (int i = y_off; i < c3_gt.height() - y_off; ++i)
    {
        for (int j = x_off; j < c3_gt.width() - x_off; ++j)
        {
            c3_gt_crop(j-x_off, i-y_off, 0) = c3_gt(j, i, 0);
            c3_gt_crop(j-x_off, i-y_off, 1) = c3_gt(j, i, 1);
            c3_gt_crop(j-x_off, i-y_off, 2) = c3_gt(j, i, 2);

            c3_rat_crop(j-x_off, i-y_off, 0) = c3_rat(j, i, 0);
            c3_rat_crop(j-x_off, i-y_off, 1) = c3_rat(j, i, 1);
            c3_rat_crop(j-x_off, i-y_off, 2) = c3_rat(j, i, 2);

            c3_tay_crop(j-x_off, i-y_off, 0) = c3_tay(j, i, 0);
            c3_tay_crop(j-x_off, i-y_off, 1) = c3_tay(j, i, 1);
            c3_tay_crop(j-x_off, i-y_off, 2) = c3_tay(j, i, 2);

            c8_gt_crop(j-x_off, i-y_off, 0) = c8_gt(j, i, 0);
            c8_gt_crop(j-x_off, i-y_off, 1) = c8_gt(j, i, 1);
            c8_gt_crop(j-x_off, i-y_off, 2) = c8_gt(j, i, 2);

            c8_rat_crop(j-x_off, i-y_off, 0) = c8_rat(j, i, 0);
            c8_rat_crop(j-x_off, i-y_off, 1) = c8_rat(j, i, 1);
            c8_rat_crop(j-x_off, i-y_off, 2) = c8_rat(j, i, 2);

            c8_tay_crop(j-x_off, i-y_off, 0) = c8_tay(j, i, 0);
            c8_tay_crop(j-x_off, i-y_off, 1) = c8_tay(j, i, 1);
            c8_tay_crop(j-x_off, i-y_off, 2) = c8_tay(j, i, 2);
        }
    }

    imedit::write_image("debiasing_figs_images/smoke/c3_gt.png", c3_gt_crop);
    imedit::write_image("debiasing_figs_images/smoke/c3_rat.png", c3_rat_crop);
    imedit::write_image("debiasing_figs_images/smoke/c3_tay.png", c3_tay_crop);

    imedit::write_image("debiasing_figs_images/smoke/c8_gt.png", c8_gt_crop);
    imedit::write_image("debiasing_figs_images/smoke/c8_rat.png", c8_rat_crop);
    imedit::write_image("debiasing_figs_images/smoke/c8_tay.png", c8_tay_crop);

    Image c3_rat_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    Image c3_tay_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    Image c3_gt_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    Image c8_rat_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    Image c8_gt_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    Image c8_tay_inset = Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);

    int c3_j_off = double(c3_gt_crop.width()) * 0.40;
    int c3_i_off = double(c3_gt_crop.height()) * 0.30;
    int c8_j_off = double(c3_gt_crop.width()) * 0.70;
    int c8_i_off = double(c3_gt_crop.height()) * 0.43;

    for (int i = 0; i < c3_rat_inset.height(); ++i)
    {
        for (int j = 0; j < c3_rat_inset.width(); ++j)
        {
            c3_rat_inset(j, i, 0) = c3_rat_crop(j+c3_j_off, i+c3_i_off, 0);
            c3_rat_inset(j, i, 1) = c3_rat_crop(j+c3_j_off, i+c3_i_off, 1);
            c3_rat_inset(j, i, 2) = c3_rat_crop(j+c3_j_off, i+c3_i_off, 2);

            c3_tay_inset(j, i, 0) = c3_tay_crop(j+c3_j_off, i+c3_i_off, 0);
            c3_tay_inset(j, i, 1) = c3_tay_crop(j+c3_j_off, i+c3_i_off, 1);
            c3_tay_inset(j, i, 2) = c3_tay_crop(j+c3_j_off, i+c3_i_off, 2);

            c3_gt_inset(j, i, 0) = c3_gt_crop(j+c3_j_off, i+c3_i_off, 0);
            c3_gt_inset(j, i, 1) = c3_gt_crop(j+c3_j_off, i+c3_i_off, 1);
            c3_gt_inset(j, i, 2) = c3_gt_crop(j+c3_j_off, i+c3_i_off, 2);
        }
    }

    for (int i = 0; i < c3_rat_inset.height(); ++i)
    {
        for (int j = 0; j < c3_rat_inset.width(); ++j)
        {
            c8_rat_inset(j, i, 0) = c8_rat_crop(j+c8_j_off, i+c8_i_off, 0);
            c8_rat_inset(j, i, 1) = c8_rat_crop(j+c8_j_off, i+c8_i_off, 1);
            c8_rat_inset(j, i, 2) = c8_rat_crop(j+c8_j_off, i+c8_i_off, 2);

            c8_tay_inset(j, i, 0) = c8_tay_crop(j+c8_j_off, i+c8_i_off, 0);
            c8_tay_inset(j, i, 1) = c8_tay_crop(j+c8_j_off, i+c8_i_off, 1);
            c8_tay_inset(j, i, 2) = c8_tay_crop(j+c8_j_off, i+c8_i_off, 2);

            c8_gt_inset(j, i, 0) = c8_gt_crop(j+c3_j_off, i+c3_i_off, 0);
            c8_gt_inset(j, i, 1) = c8_gt_crop(j+c3_j_off, i+c3_i_off, 1);
            c8_gt_inset(j, i, 2) = c8_gt_crop(j+c3_j_off, i+c3_i_off, 2);
        }
    }

    // c8_rat_inset.exposure(5.0);
    // c8_tay_inset.exposure(5.0);

    std::cout << "rmse_rat_0.8: " << imedit::root_mean_sqr_error(c8_gt_crop, c8_rat_crop) << std::endl;
    std::cout << "rmse_tay_0.8: " << imedit::root_mean_sqr_error(c8_gt_crop, c8_tay_crop) << std::endl;
    std::cout << "rmse_rat_0.3: " << imedit::root_mean_sqr_error(c3_gt_crop, c3_rat_crop) << std::endl;
    std::cout << "rmse_tay_0.3: " << imedit::root_mean_sqr_error(c3_gt_crop, c3_tay_crop) << std::endl;

    std::cout << "rmse_rat_0.8 inset: " << imedit::root_mean_sqr_error(c8_gt_inset, c8_rat_inset) << std::endl;
    std::cout << "rmse_tay_0.8 inset: " << imedit::root_mean_sqr_error(c8_gt_inset, c8_tay_inset) << std::endl;
    std::cout << "rmse_rat_0.3 inset: " << imedit::root_mean_sqr_error(c3_gt_inset, c3_rat_inset) << std::endl;
    std::cout << "rmse_tay_0.3 inset: " << imedit::root_mean_sqr_error(c3_gt_inset, c3_tay_inset) << std::endl;

    // c8_rat_inset.exposure(5.0);
    // c8_tay_inset.exposure(5.0);

    imedit::write_image("debiasing_figs_images/smoke/c3_rat_inset.png", c3_rat_inset);
    imedit::write_image("debiasing_figs_images/smoke/c3_tay_inset.png", c3_tay_inset);
    imedit::write_image("debiasing_figs_images/smoke/c8_rat_inset.png", c3_tay_inset);
    imedit::write_image("debiasing_figs_images/smoke/c8_tay_inset.png", c3_tay_inset);
}

void create_cloud_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/cloud/";

    system("mkdir debiasing_figs_images/cloud/");

    Image exp_anal = Image(path + "exp_anal.exr");
    Image c_1_b_0_25_anal = Image(path + "c_1_0_b_0_25_anal.exr");
    Image c_2_b_0_5_anal = Image(path + "c_2_0_b_0_5_anal.exr");
    Image c_1_5_b_0_75_anal = Image(path + "c_1_5_b_0_75_anal.exr");
    Image exp_deb = Image(path + "exp_deb.exr");
    Image c_1_b_0_25_deb = Image(path + "c_1_0_b_0_25_deb.exr");
    Image c_2_b_0_5_deb = Image(path + "c_2_0_b_0_5_deb.exr");
    Image c_1_5_b_0_75_deb = Image(path + "c_1_5_b_0_75_deb.exr");

    // TODO: make smaller... maybe

    imedit::write_image("debiasing_figs_images/cloud/exp_anal.png", exp_anal);
    imedit::write_image("debiasing_figs_images/cloud/c_1_0_b_0_25_anal.png", c_1_b_0_25_anal);
    imedit::write_image("debiasing_figs_images/cloud/c_2_0_b_0_5_anal.png", c_2_b_0_5_anal);
    imedit::write_image("debiasing_figs_images/cloud/c_1_5_b_0_75_anal.png", c_1_5_b_0_75_anal);
    imedit::write_image("debiasing_figs_images/cloud/exp_deb.png", exp_deb);
    imedit::write_image("debiasing_figs_images/cloud/c_1_0_b_0_25_deb.png", c_1_b_0_25_deb);
    imedit::write_image("debiasing_figs_images/cloud/c_2_0_b_0_5_deb.png", c_2_b_0_5_deb);
    imedit::write_image("debiasing_figs_images/cloud/c_1_5_b_0_75_deb.png", c_1_5_b_0_75_deb);
}

void do_crop(
    const Image& one,
    const Image& two,
    Image& crop_one,
    Image& crop_two
)
{
    int i_start = 0;
    int j_start = 0;
    bool found = false;

    for (int i = 0; i < two.height(); ++i)
    {
        for (int j = 0; j < two.width(); ++j)
        {
            if (!found)
            {
                if (two(j, i, 0) != 0.0 || two(j, i, 1) != 0.0 || two(j, i, 2) != 0.0)
                {
                    i_start = i;
                    j_start = j;
                    found = true;
                }
            }
        }
    }

    for (int i = 0; i < crop_two.height(); ++i)
    {
        for (int j = 0; j < crop_two.width(); ++j)
        {
            crop_one(j, i, 0) = one(j+j_start, i+i_start, 0);
            crop_one(j, i, 1) = one(j+j_start, i+i_start, 1);
            crop_one(j, i, 2) = one(j+j_start, i+i_start, 2);

            crop_two(j, i, 0) = two(j+j_start, i+i_start, 0);
            crop_two(j, i, 1) = two(j+j_start, i+i_start, 1);
            crop_two(j, i, 2) = two(j+j_start, i+i_start, 2);
        }
    }
}

void create_cloud_2_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/cloud/";

    system("mkdir debiasing_figs_images/cloud_2/");

    Image exp_anal = Image(path + "exp_anal.exr");
    Image c_1_b_0_25_anal = Image(path + "c_1_0_b_0_25_anal.exr");
    Image c_2_b_0_5_anal = Image(path + "c_2_0_b_0_5_anal.exr");
    Image c_1_5_b_0_75_anal = Image(path + "c_1_5_b_0_75_anal.exr");
    Image exp_deb = Image(path + "exp_deb.exr");
    Image c_1_b_0_25_deb = Image(path + "c_1_0_b_0_25_deb.exr");
    Image c_2_b_0_5_deb = Image(path + "c_2_0_b_0_5_deb.exr");
    Image c_1_5_b_0_75_deb = Image(path + "c_1_5_b_0_75_deb.exr");

    Image ray_1 = Image(path + "ray_1.exr");
    Image ray_2 = Image(path + "ray_2.exr");
    Image ray_3 = Image(path + "ray_3_1.exr");
    Image ray_4 = Image(path + "ray_4.exr");

    Image crop_1_deb = Image(40, 40);
    Image crop_2_deb = Image(30, 30);
    Image crop_3_deb = Image(30, 30);
    Image crop_4_deb = Image(30, 30);
    Image crop_1_ray = Image(40, 40);
    Image crop_2_ray = Image(30, 30);
    Image crop_3_ray = Image(30, 30);
    Image crop_4_ray = Image(30, 30);

    do_crop(exp_deb, ray_1, crop_1_deb, crop_1_ray);
    do_crop(c_1_b_0_25_deb, ray_2, crop_2_deb, crop_2_ray);
    do_crop(c_2_b_0_5_deb, ray_3, crop_3_deb, crop_3_ray);
    do_crop(c_1_5_b_0_75_deb, ray_4, crop_4_deb, crop_4_ray);

    // 100 off top
    // 150 off bottom
    // 70 right
    // 90 left
    int new_width = exp_anal.width() - 70 - 90;
    int new_height = exp_anal.height() - 95 - 145;

    Image mod_exp_anal = Image(new_width, new_height);
    Image mod_c_1_b_0_25_anal = Image(new_width, new_height);
    Image mod_c_2_b_0_5_anal = Image(new_width, new_height);
    Image mod_c_1_5_b_0_75_anal = Image(new_width, new_height);
    Image mod_exp_deb = Image(new_width, new_height);
    Image mod_c_1_b_0_25_deb = Image(new_width, new_height);
    Image mod_c_2_b_0_5_deb = Image(new_width, new_height);
    Image mod_c_1_5_b_0_75_deb = Image(new_width, new_height);

    for (int i = 0; i < new_height; ++i)
    {
        for (int j = 0; j < new_width; ++j)
        {
            int jj = j + 90;
            int ii = i + 95; // 100

            mod_exp_anal(j, i, 0) = exp_anal(jj, ii, 0);
            mod_exp_anal(j, i, 1) = exp_anal(jj, ii, 1);
            mod_exp_anal(j, i, 2) = exp_anal(jj, ii, 2);

            mod_c_1_b_0_25_anal(j, i, 0) = c_1_b_0_25_anal(jj, ii, 0);
            mod_c_1_b_0_25_anal(j, i, 1) = c_1_b_0_25_anal(jj, ii, 1);
            mod_c_1_b_0_25_anal(j, i, 2) = c_1_b_0_25_anal(jj, ii, 2);

            mod_c_2_b_0_5_anal(j, i, 0) = c_2_b_0_5_anal(jj, ii, 0);
            mod_c_2_b_0_5_anal(j, i, 1) = c_2_b_0_5_anal(jj, ii, 1);
            mod_c_2_b_0_5_anal(j, i, 2) = c_2_b_0_5_anal(jj, ii, 2);

            mod_c_1_5_b_0_75_anal(j, i, 0) = c_1_5_b_0_75_anal(jj, ii, 0);
            mod_c_1_5_b_0_75_anal(j, i, 1) = c_1_5_b_0_75_anal(jj, ii, 1);
            mod_c_1_5_b_0_75_anal(j, i, 2) = c_1_5_b_0_75_anal(jj, ii, 2);

            mod_exp_deb(j, i, 0) = exp_deb(jj, ii, 0);
            mod_exp_deb(j, i, 1) = exp_deb(jj, ii, 1);
            mod_exp_deb(j, i, 2) = exp_deb(jj, ii, 2);

            mod_c_1_b_0_25_deb(j, i, 0) = c_1_b_0_25_deb(jj, ii, 0);
            mod_c_1_b_0_25_deb(j, i, 1) = c_1_b_0_25_deb(jj, ii, 1);
            mod_c_1_b_0_25_deb(j, i, 2) = c_1_b_0_25_deb(jj, ii, 2);

            mod_c_2_b_0_5_deb(j, i, 0) = c_2_b_0_5_deb(jj, ii, 0);
            mod_c_2_b_0_5_deb(j, i, 1) = c_2_b_0_5_deb(jj, ii, 1);
            mod_c_2_b_0_5_deb(j, i, 2) = c_2_b_0_5_deb(jj, ii, 2);

            mod_c_1_5_b_0_75_deb(j, i, 0) = c_1_5_b_0_75_deb(jj, ii, 0);
            mod_c_1_5_b_0_75_deb(j, i, 1) = c_1_5_b_0_75_deb(jj, ii, 1);
            mod_c_1_5_b_0_75_deb(j, i, 2) = c_1_5_b_0_75_deb(jj, ii, 2);
        }
    }

    std::cout << "new_height: " << new_height << std::endl;
    std::cout << "new_width: " << new_width << std::endl;

    int wid = mod_exp_deb.width();
    int hei = mod_exp_deb.height();

    Image final_image(wid, hei);

    for (int i = 0; i < hei; ++i)
    {
        for (int j = 0; j < wid; ++j)
        {
            // if (j < 159)
            // {
            //     final_image(j, i, 0) = mod_exp_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_exp_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_exp_deb(j, i, 2);
            // }
            // else if (j > 161 && j < 319)
            // {
            //     final_image(j, i, 0) = mod_c_1_b_0_25_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_b_0_25_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_b_0_25_deb(j, i, 2);
            // }
            //
            // else if (j > 321 && j < 479)
            // {
            //     final_image(j, i, 0) = mod_c_2_b_0_5_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_2_b_0_5_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_2_b_0_5_deb(j, i, 2);
            // }
            //
            // else if (j > 481 && j < 640)
            // {
            //     final_image(j, i, 0) = mod_c_1_5_b_0_75_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_5_b_0_75_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_5_b_0_75_deb(j, i, 2);
            // }
            // else
            // {
            //     final_image(j, i, 0) = 0.3;
            //     final_image(j, i, 1) = 0.3;
            //     final_image(j, i, 2) = 0.3;
            // }

            // if (j < 160)
            // {
            //     final_image(j, i, 0) = mod_exp_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_exp_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_exp_deb(j, i, 2);
            // }
            // else if (j < 320)
            // {
            //     final_image(j, i, 0) = mod_c_1_b_0_25_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_b_0_25_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_b_0_25_deb(j, i, 2);
            // }
            //
            // else if (j < 480)
            // {
            //     final_image(j, i, 0) = mod_c_2_b_0_5_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_2_b_0_5_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_2_b_0_5_deb(j, i, 2);
            // }
            //
            // else if (j < 640)
            // {
            //     final_image(j, i, 0) = mod_c_1_5_b_0_75_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_5_b_0_75_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_5_b_0_75_deb(j, i, 2);
            // }
            // else
            // {
            //     final_image(j, i, 0) = 0.3;
            //     final_image(j, i, 1) = 0.3;
            //     final_image(j, i, 2) = 0.3;
            // }

            // if (i < 90)
            // {
            //     final_image(j, i, 0) = mod_exp_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_exp_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_exp_deb(j, i, 2);
            // }
            // else if (i < 180)
            // {
            //     final_image(j, i, 0) = mod_c_1_b_0_25_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_b_0_25_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_b_0_25_deb(j, i, 2);
            // }
            //
            // else if (i < 270)
            // {
            //     final_image(j, i, 0) = mod_c_2_b_0_5_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_2_b_0_5_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_2_b_0_5_deb(j, i, 2);
            // }
            //
            // else if (i < 360)
            // {
            //     final_image(j, i, 0) = mod_c_1_5_b_0_75_deb(j, i, 0);
            //     final_image(j, i, 1) = mod_c_1_5_b_0_75_deb(j, i, 1);
            //     final_image(j, i, 2) = mod_c_1_5_b_0_75_deb(j, i, 2);
            // }
            // else
            // {
            //     final_image(j, i, 0) = 0.3;
            //     final_image(j, i, 1) = 0.3;
            //     final_image(j, i, 2) = 0.3;
            // }
            double trans = 0.05;

            if (i == 90 || i == 89 || i == 91)
            {
                final_image(j, i, 0) = trans + (1.0 - trans) * mod_exp_deb(j, i, 0);
                final_image(j, i, 1) = trans + (1.0 - trans) * mod_exp_deb(j, i, 1);
                final_image(j, i, 2) = trans + (1.0 - trans) * mod_exp_deb(j, i, 2);
            }
            else if (i == 180 || i == 179 || i == 181)
            {
                final_image(j, i, 0) = trans + (1.0 - trans) * mod_c_1_b_0_25_deb(j, i, 0);
                final_image(j, i, 1) = trans + (1.0 - trans) * mod_c_1_b_0_25_deb(j, i, 1);
                final_image(j, i, 2) = trans + (1.0 - trans) * mod_c_1_b_0_25_deb(j, i, 2);
            }
            else if (i == 270 || i == 269|| i == 271)
            {
                final_image(j, i, 0) = trans + (1.0 - trans) * mod_c_2_b_0_5_deb(j, i, 0);
                final_image(j, i, 1) = trans + (1.0 - trans) * mod_c_2_b_0_5_deb(j, i, 1);
                final_image(j, i, 2) = trans + (1.0 - trans) * mod_c_2_b_0_5_deb(j, i, 2);
            }
            else if (i <= 89)
            {
                final_image(j, i, 0) = mod_exp_deb(j, i, 0);
                final_image(j, i, 1) = mod_exp_deb(j, i, 1);
                final_image(j, i, 2) = mod_exp_deb(j, i, 2);
            }
            else if (i <= 179)
            {
                final_image(j, i, 0) = mod_c_1_b_0_25_deb(j, i, 0);
                final_image(j, i, 1) = mod_c_1_b_0_25_deb(j, i, 1);
                final_image(j, i, 2) = mod_c_1_b_0_25_deb(j, i, 2);
            }
            else if (i <= 269)
            {
                final_image(j, i, 0) = mod_c_2_b_0_5_deb(j, i, 0);
                final_image(j, i, 1) = mod_c_2_b_0_5_deb(j, i, 1);
                final_image(j, i, 2) = mod_c_2_b_0_5_deb(j, i, 2);
            }
            else
            {
                final_image(j, i, 0) = mod_c_1_5_b_0_75_deb(j, i, 0);
                final_image(j, i, 1) = mod_c_1_5_b_0_75_deb(j, i, 1);
                final_image(j, i, 2) = mod_c_1_5_b_0_75_deb(j, i, 2);
            }
        }
    }

    imedit::write_image("debiasing_figs_images/cloud_2/combined.exr", final_image);
    imedit::write_image("debiasing_figs_images/cloud_2/combined.png", final_image);

    std::cout << "width: " << wid << std::endl;
    std::cout << "height: " << hei << std::endl;

    imedit::write_image("debiasing_figs_images/cloud_2/exp_anal.png", mod_exp_anal);
    imedit::write_image("debiasing_figs_images/cloud_2/c_1_0_b_0_25_anal.png", mod_c_1_b_0_25_anal);
    imedit::write_image("debiasing_figs_images/cloud_2/c_2_0_b_0_5_anal.png", mod_c_2_b_0_5_anal);
    imedit::write_image("debiasing_figs_images/cloud_2/c_1_5_b_0_75_anal.png", mod_c_1_5_b_0_75_anal);
    imedit::write_image("debiasing_figs_images/cloud_2/exp_deb.png", mod_exp_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/c_1_0_b_0_25_deb.png", mod_c_1_b_0_25_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/c_2_0_b_0_5_deb.png", mod_c_2_b_0_5_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/c_1_5_b_0_75_deb.png", mod_c_1_5_b_0_75_deb);

    imedit::write_image("debiasing_figs_images/cloud_2/deb_crop_1.png", crop_1_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/deb_crop_2.png", crop_2_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/deb_crop_3.png", crop_3_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/deb_crop_4.png", crop_4_deb);
    imedit::write_image("debiasing_figs_images/cloud_2/ray_crop_1.png", crop_1_ray);
    imedit::write_image("debiasing_figs_images/cloud_2/ray_crop_2.png", crop_2_ray);
    imedit::write_image("debiasing_figs_images/cloud_2/ray_crop_3.png", crop_3_ray);
    imedit::write_image("debiasing_figs_images/cloud_2/ray_crop_4.png", crop_4_ray);
}

void create_consistent_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/consist/";

    system("mkdir debiasing_figs_images/consistent/");

    Image scene = Image(path + "final.exr");
    // Image fd_ref = Image(path + "final_fd.exr");
    Image fd_ref = Image(path + "consist_8192.exr");
    Image consist_3 = Image(path + "consist_8192.exr");
    Image consist_2 = Image(path + "consist_256.exr");
    Image consist_1 = Image(path + "consist_16.exr");

    Image mod_fd = Image(fd_ref.width(), fd_ref.height());
    Image mod_consist_3 = Image(consist_3.width(), consist_3.height());
    Image mod_consist_2 = Image(consist_2.width(), consist_2.height());
    Image mod_consist_1 = Image(consist_1.width(), consist_1.height());

    create_fd_image(fd_ref, mod_fd);
    create_fd_image(consist_3, mod_consist_3);
    create_fd_image(consist_2, mod_consist_2);
    create_fd_image(consist_1, mod_consist_1);

    Image consist_1_inset = Image(fd_ref.width()/10, fd_ref.height()/10);
    Image consist_2_inset = Image(fd_ref.width()/10, fd_ref.height()/10);
    Image consist_3_inset = Image(fd_ref.width()/10, fd_ref.height()/10);

    std::cout << "hello" << std::endl;

    int inset_j_off = double(fd_ref.width()) * 0.05;
    int inset_i_off = double(fd_ref.height()) * 0.05;

    for (int i = 0; i < consist_1_inset.height(); ++i)
    {
        for (int j = 0; j < consist_1_inset.width(); ++j)
        {
            consist_1_inset(j, i, 0) = mod_consist_1(j + inset_j_off, i + inset_i_off, 0);
            consist_1_inset(j, i, 1) = mod_consist_1(j + inset_j_off, i + inset_i_off, 1);
            consist_1_inset(j, i, 2) = mod_consist_1(j + inset_j_off, i + inset_i_off, 2);

            consist_2_inset(j, i, 0) = mod_consist_2(j + inset_j_off, i + inset_i_off, 0);
            consist_2_inset(j, i, 1) = mod_consist_2(j + inset_j_off, i + inset_i_off, 1);
            consist_2_inset(j, i, 2) = mod_consist_2(j + inset_j_off, i + inset_i_off, 2);

            consist_3_inset(j, i, 0) = mod_consist_3(j + inset_j_off, i + inset_i_off, 0);
            consist_3_inset(j, i, 1) = mod_consist_3(j + inset_j_off, i + inset_i_off, 1);
            consist_3_inset(j, i, 2) = mod_consist_3(j + inset_j_off, i + inset_i_off, 2);
        }
    }
    
    imedit::write_image("debiasing_figs_images/consistent/scene.exr", scene);
    imedit::write_image("debiasing_figs_images/consistent/fd_ref.exr", mod_fd);
    imedit::write_image("debiasing_figs_images/consistent/consist_3.exr", mod_consist_3);
    imedit::write_image("debiasing_figs_images/consistent/consist_2.exr", mod_consist_2);
    imedit::write_image("debiasing_figs_images/consistent/consist_1.exr", mod_consist_1);

    imedit::write_image("debiasing_figs_images/consistent/scene.png", scene);
    imedit::write_image("debiasing_figs_images/consistent/fd_ref.png", mod_fd);
    imedit::write_image("debiasing_figs_images/consistent/consist_3.png", mod_consist_3);
    imedit::write_image("debiasing_figs_images/consistent/consist_2.png", mod_consist_2);
    imedit::write_image("debiasing_figs_images/consistent/consist_1.png", mod_consist_1);
}

void create_shader_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/shader/";

    system("mkdir debiasing_figs_images/shader/");

    Image scene = Image(path + "real-final.exr");
    Image biased = Image(path + "shader_biased.exr");
    Image ref = Image(path + "shader_ref.exr");
    Image unb = Image(path + "shader_unb.exr");

    Image mod_scene = Image(scene.width(), scene.height());
    Image mod_biased = Image(biased.width(), biased.height());
    Image mod_ref = Image(ref.width(), ref.height());
    Image mod_unb = Image(unb.width(), unb.height());

    create_fd_image(scene, mod_scene);
    create_fd_image(biased, mod_biased);
    create_fd_image(ref, mod_ref);
    create_fd_image(unb, mod_unb);

    imedit::write_image("debiasing_figs_images/shader/scene.exr", scene);
    imedit::write_image("debiasing_figs_images/shader/preexp_biased.exr", mod_biased);
    imedit::write_image("debiasing_figs_images/shader/preexp_ref.exr", mod_ref);
    imedit::write_image("debiasing_figs_images/shader/preexp_unb.exr", mod_unb);

    int i1_offset = 200;
    int j1_offset = 175;
    int i2_offset = 345;
    int j2_offset = 125;

    int wid = 50;
    int hei = 35;

    Image biased_inset_1 = Image(wid, hei);
    Image ref_inset_1 = Image(wid, hei);
    Image unb_inset_1 = Image(wid, hei);

    Image biased_inset_2 = Image(wid, hei);
    Image ref_inset_2 = Image(wid, hei);
    Image unb_inset_2 = Image(wid, hei);

    for (int i = 0; i < hei; ++i)
    {
        for (int j = 0; j < wid; ++j)
        {
            biased_inset_1(j, i, 0) = mod_biased(j+j1_offset, i+i1_offset, 0);
            biased_inset_1(j, i, 1) = mod_biased(j+j1_offset, i+i1_offset, 1);
            biased_inset_1(j, i, 2) = mod_biased(j+j1_offset, i+i1_offset, 2);

            ref_inset_1(j, i, 0) = mod_ref(j+j1_offset, i+i1_offset, 0);
            ref_inset_1(j, i, 1) = mod_ref(j+j1_offset, i+i1_offset, 1);
            ref_inset_1(j, i, 2) = mod_ref(j+j1_offset, i+i1_offset, 2);

            unb_inset_1(j, i, 0) = mod_unb(j+j1_offset, i+i1_offset, 0);
            unb_inset_1(j, i, 1) = mod_unb(j+j1_offset, i+i1_offset, 1);
            unb_inset_1(j, i, 2) = mod_unb(j+j1_offset, i+i1_offset, 2);

            biased_inset_2(j, i, 0) = mod_biased(j+j2_offset, i+i2_offset, 0);
            biased_inset_2(j, i, 1) = mod_biased(j+j2_offset, i+i2_offset, 1);
            biased_inset_2(j, i, 2) = mod_biased(j+j2_offset, i+i2_offset, 2);

            ref_inset_2(j, i, 0) = mod_ref(j+j2_offset, i+i2_offset, 0);
            ref_inset_2(j, i, 1) = mod_ref(j+j2_offset, i+i2_offset, 1);
            ref_inset_2(j, i, 2) = mod_ref(j+j2_offset, i+i2_offset, 2);

            unb_inset_2(j, i, 0) = mod_unb(j+j2_offset, i+i2_offset, 0);
            unb_inset_2(j, i, 1) = mod_unb(j+j2_offset, i+i2_offset, 1);
            unb_inset_2(j, i, 2) = mod_unb(j+j2_offset, i+i2_offset, 2);
        }
    }

    mod_biased.exposure(1.0 / 1000.0);
    mod_ref.exposure(1.0 / 1000.0);
    mod_unb.exposure(1.0 / 1000.0);

    biased_inset_1.exposure(1.0 / 1000.0);
    ref_inset_1.exposure(1.0 / 1000.0);
    unb_inset_1.exposure(1.0 / 1000.0);

    biased_inset_2.exposure(1.0 / 1000.0);
    ref_inset_2.exposure(1.0 / 1000.0);
    unb_inset_2.exposure(1.0 / 1000.0);

    imedit::write_image("debiasing_figs_images/shader/biased.exr", mod_biased);
    imedit::write_image("debiasing_figs_images/shader/ref.exr", mod_ref);
    imedit::write_image("debiasing_figs_images/shader/unb.exr", mod_unb);

    imedit::write_image("debiasing_figs_images/shader/scene.png", scene);
    imedit::write_image("debiasing_figs_images/shader/biased.png", mod_biased);
    imedit::write_image("debiasing_figs_images/shader/ref.png", mod_ref);
    imedit::write_image("debiasing_figs_images/shader/unb.png", mod_unb);
    
    imedit::write_image("debiasing_figs_images/shader/biased_inset_1.png", biased_inset_1);
    imedit::write_image("debiasing_figs_images/shader/ref_inset_1.png", ref_inset_1);
    imedit::write_image("debiasing_figs_images/shader/unb_inset_1.png", unb_inset_1);
    imedit::write_image("debiasing_figs_images/shader/biased_inset_2.png", biased_inset_2);
    imedit::write_image("debiasing_figs_images/shader/ref_inset_2.png", ref_inset_2);
    imedit::write_image("debiasing_figs_images/shader/unb_inset_2.png", unb_inset_2);

    imedit::write_image("debiasing_figs_images/shader/biased_inset_1.exr", biased_inset_1);
    imedit::write_image("debiasing_figs_images/shader/ref_inset_1.exr", ref_inset_1);
    imedit::write_image("debiasing_figs_images/shader/unb_inset_1.exr", unb_inset_1);
    imedit::write_image("debiasing_figs_images/shader/biased_inset_2.exr", biased_inset_2);
    imedit::write_image("debiasing_figs_images/shader/ref_inset_2.exr", ref_inset_2);
    imedit::write_image("debiasing_figs_images/shader/unb_inset_2.exr", unb_inset_2);
}

void create_teaser_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/teaser/";

    system("mkdir debiasing_figs_images/teaser/");

    Image teaser = Image(path + "base_teaser.exr");
    Image fd = Image(path + "fd_teaser.exr");

    Image mod_fd = Image(fd.width(), fd.height());

    create_fd_image(fd, mod_fd);

    // for (int i = 0; i < mod_fd.height(); ++i)
    // {
    //     for (int j = 0; j < mod_fd.width(); ++j)
    //     {
    //         double lum = (fd(j, i, 0) * 0.3) + (fd(j, i, 1) * 0.59) + (fd(j, i, 2) * 0.11);
    //
    //         if (lum < 0.0)
    //         {
    //             mod_fd(j, i, 2) = -lum;
    //         }
    //         else
    //         {
    //             mod_fd(j, i, 0) = lum;
    //         }
    //     }
    // }

    mod_fd.exposure(12.5);

    Image combined = Image(fd.width(), fd.height());

    for (int i = 0; i < mod_fd.height(); ++i)
    {
        for (int j = 0; j < mod_fd.width(); ++j)
        {
            int y_loc = int(720.0 - 640.0 / 1600.0 * double(j));

            if (std::abs(i - y_loc) <= 3)
            {
                combined(j, i, 0) = 0.0;
                combined(j, i, 1) = 0.0;
                combined(j, i, 2) = 0.0;
            }
            else if (i < y_loc)
            {
                combined(j, i, 0) = mod_fd(j, i, 0);
                combined(j, i, 1) = mod_fd(j, i, 1);
                combined(j, i, 2) = mod_fd(j, i, 2);
            }
            else
            {
                combined(j, i, 0) = teaser(j, i, 0);
                combined(j, i, 1) = teaser(j, i, 1);
                combined(j, i, 2) = teaser(j, i, 2);
            }
        }
    }

    imedit::write_image("debiasing_figs_images/teaser/teaser_base.png", teaser);
    imedit::write_image("debiasing_figs_images/teaser/teaser_fd.png", mod_fd);
    imedit::write_image("debiasing_figs_images/teaser/combined_teaser.png", combined);
}

void create_da_figure()
{
    // TODO
}

int main()
{
    system("rm -rf debiasing_figs_images/");
    system("mkdir debiasing_figs_images/");

    create_didactic_figs();
    create_smoke_figure();
    create_cloud_figure();
    create_cloud_2_figure();
    create_consistent_figure();
    create_teaser_figure();
    create_shader_figure();
    create_da_figure();
}
