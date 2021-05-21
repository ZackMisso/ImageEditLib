#include <iostream>
#include <imedit/image.h>
#include <imedit/im_util.h>
#include <fstream>
#include <sstream>

void create_fd_image(const imedit::Image& fd, imedit::Image& image)
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

    imedit::Image gt = imedit::Image(path + "gt.exr");
    imedit::Image bounce_1 = imedit::Image(path + "bounce_1.exr");
    imedit::Image bounce_2 = imedit::Image(path + "bounce_2.exr");
    imedit::Image bounce_3 = imedit::Image(path + "bounce_3.exr");
    imedit::Image bounce_4 = imedit::Image(path + "bounce_4.exr");
    imedit::Image bounce_5 = imedit::Image(path + "bounce_5.exr");
    imedit::Image bounce_6 = imedit::Image(path + "bounce_6.exr");
    imedit::Image bounce_7 = imedit::Image(path + "bounce_7.exr");

    imedit::Image delta_1 = bounce_1;
    imedit::Image delta_2 = bounce_2 - bounce_1;
    imedit::Image delta_3 = bounce_3 - bounce_2;
    imedit::Image delta_4 = bounce_4 - bounce_3;
    imedit::Image delta_5 = bounce_5 - bounce_4;
    imedit::Image delta_6 = bounce_6 - bounce_5;
    imedit::Image delta_7 = bounce_7 - bounce_6;

    // TODO: do more manipulations if necessary

    gt.write("debiasing_figs_images/didactic/gt.png");
    delta_1.write("debiasing_figs_images/didactic/delta_1.png");
    delta_2.write("debiasing_figs_images/didactic/delta_2.png");
    delta_3.write("debiasing_figs_images/didactic/delta_3.png");
    delta_4.write("debiasing_figs_images/didactic/delta_4.png");
    delta_5.write("debiasing_figs_images/didactic/delta_5.png");
    delta_6.write("debiasing_figs_images/didactic/delta_6.png");
    delta_7.write("debiasing_figs_images/didactic/delta_7.png");
}

void create_smoke_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/smoke/";

    system("mkdir debiasing_figs_images/smoke/");

    imedit::Image c3_gt = imedit::Image(path + "c3_gt.exr");
    imedit::Image c8_gt = imedit::Image(path + "c8_gt.exr");
    imedit::Image c3_rat = imedit::Image(path + "c3_rat.exr");
    imedit::Image c3_tay = imedit::Image(path + "c3_tay.exr");
    imedit::Image c8_rat = imedit::Image(path + "c8_rat.exr");
    imedit::Image c8_tay = imedit::Image(path + "c8_tay.exr");

    int width_cut = 9*8;
    int height_cut = 11*8;

    imedit::Image c3_gt_crop = imedit::Image(c3_gt.width() - width_cut, c3_gt.height() - height_cut);
    imedit::Image c3_rat_crop = imedit::Image(c3_rat.width() - width_cut, c3_rat.height() - height_cut);
    imedit::Image c3_tay_crop = imedit::Image(c3_tay.width() - width_cut, c3_tay.height() - height_cut);
    imedit::Image c8_gt_crop = imedit::Image(c8_gt.width() - width_cut, c8_gt.height() - height_cut);
    imedit::Image c8_rat_crop = imedit::Image(c8_rat.width() - width_cut, c8_rat.height() - height_cut);
    imedit::Image c8_tay_crop = imedit::Image(c8_tay.width() - width_cut, c8_tay.height() - height_cut);

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

    c3_gt_crop.write("debiasing_figs_images/smoke/c3_gt.png");
    c3_rat_crop.write("debiasing_figs_images/smoke/c3_rat.png");
    c3_tay_crop.write("debiasing_figs_images/smoke/c3_tay.png");

    c8_gt_crop.write("debiasing_figs_images/smoke/c8_gt.png");
    c8_rat_crop.write("debiasing_figs_images/smoke/c8_rat.png");
    c8_tay_crop.write("debiasing_figs_images/smoke/c8_tay.png");

    imedit::Image c3_rat_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    imedit::Image c3_tay_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    imedit::Image c3_gt_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    imedit::Image c8_rat_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    imedit::Image c8_gt_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);
    imedit::Image c8_tay_inset = imedit::Image(c3_gt_crop.width() / 10 * 1, c3_gt_crop.height() / 10 * 1);

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

    c3_rat_inset.write("debiasing_figs_images/smoke/c3_rat_inset.png");
    c3_tay_inset.write("debiasing_figs_images/smoke/c3_tay_inset.png");
    c8_rat_inset.write("debiasing_figs_images/smoke/c8_rat_inset.png");
    c8_tay_inset.write("debiasing_figs_images/smoke/c8_tay_inset.png");
}

void create_cloud_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/cloud/";

    system("mkdir debiasing_figs_images/cloud/");

    imedit::Image exp_anal = imedit::Image(path + "exp_anal.exr");
    imedit::Image c_1_b_0_25_anal = imedit::Image(path + "c_1_0_b_0_25_anal.exr");
    imedit::Image c_2_b_0_5_anal = imedit::Image(path + "c_2_0_b_0_5_anal.exr");
    imedit::Image c_1_5_b_0_75_anal = imedit::Image(path + "c_1_5_b_0_75_anal.exr");
    imedit::Image exp_deb = imedit::Image(path + "exp_deb.exr");
    imedit::Image c_1_b_0_25_deb = imedit::Image(path + "c_1_0_b_0_25_deb.exr");
    imedit::Image c_2_b_0_5_deb = imedit::Image(path + "c_2_0_b_0_5_deb.exr");
    imedit::Image c_1_5_b_0_75_deb = imedit::Image(path + "c_1_5_b_0_75_deb.exr");

    // TODO: make smaller... maybe

    exp_anal.write("debiasing_figs_images/cloud/exp_anal.png");
    c_1_b_0_25_anal.write("debiasing_figs_images/cloud/c_1_0_b_0_25_anal.png");
    c_2_b_0_5_anal.write("debiasing_figs_images/cloud/c_2_0_b_0_5_anal.png");
    c_1_5_b_0_75_anal.write("debiasing_figs_images/cloud/c_1_5_b_0_75_anal.png");
    exp_deb.write("debiasing_figs_images/cloud/exp_deb.png");
    c_1_b_0_25_deb.write("debiasing_figs_images/cloud/c_1_0_b_0_25_deb.png");
    c_2_b_0_5_deb.write("debiasing_figs_images/cloud/c_2_0_b_0_5_deb.png");
    c_1_5_b_0_75_deb.write("debiasing_figs_images/cloud/c_1_5_b_0_75_deb.png");
}

void do_crop(
    const imedit::Image& one,
    const imedit::Image& two,
    imedit::Image& crop_one,
    imedit::Image& crop_two
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

    imedit::Image exp_anal = imedit::Image(path + "exp_anal.exr");
    imedit::Image c_1_b_0_25_anal = imedit::Image(path + "c_1_0_b_0_25_anal.exr");
    imedit::Image c_2_b_0_5_anal = imedit::Image(path + "c_2_0_b_0_5_anal.exr");
    imedit::Image c_1_5_b_0_75_anal = imedit::Image(path + "c_1_5_b_0_75_anal.exr");
    imedit::Image exp_deb = imedit::Image(path + "exp_deb.exr");
    imedit::Image c_1_b_0_25_deb = imedit::Image(path + "c_1_0_b_0_25_deb.exr");
    imedit::Image c_2_b_0_5_deb = imedit::Image(path + "c_2_0_b_0_5_deb.exr");
    imedit::Image c_1_5_b_0_75_deb = imedit::Image(path + "c_1_5_b_0_75_deb.exr");

    imedit::Image ray_1 = imedit::Image(path + "ray_1.exr");
    imedit::Image ray_2 = imedit::Image(path + "ray_2.exr");
    imedit::Image ray_3 = imedit::Image(path + "ray_3_1.exr");
    imedit::Image ray_4 = imedit::Image(path + "ray_4.exr");

    imedit::Image crop_1_deb = imedit::Image(40, 40);
    imedit::Image crop_2_deb = imedit::Image(30, 30);
    imedit::Image crop_3_deb = imedit::Image(30, 30);
    imedit::Image crop_4_deb = imedit::Image(30, 30);
    imedit::Image crop_1_ray = imedit::Image(40, 40);
    imedit::Image crop_2_ray = imedit::Image(30, 30);
    imedit::Image crop_3_ray = imedit::Image(30, 30);
    imedit::Image crop_4_ray = imedit::Image(30, 30);

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

    imedit::Image mod_exp_anal = imedit::Image(new_width, new_height);
    imedit::Image mod_c_1_b_0_25_anal = imedit::Image(new_width, new_height);
    imedit::Image mod_c_2_b_0_5_anal = imedit::Image(new_width, new_height);
    imedit::Image mod_c_1_5_b_0_75_anal = imedit::Image(new_width, new_height);
    imedit::Image mod_exp_deb = imedit::Image(new_width, new_height);
    imedit::Image mod_c_1_b_0_25_deb = imedit::Image(new_width, new_height);
    imedit::Image mod_c_2_b_0_5_deb = imedit::Image(new_width, new_height);
    imedit::Image mod_c_1_5_b_0_75_deb = imedit::Image(new_width, new_height);

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

    imedit::Image final_image(wid, hei);

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

    final_image.write("debiasing_figs_images/cloud_2/combined.exr");
    final_image.write("debiasing_figs_images/cloud_2/combined.png");

    std::cout << "width: " << wid << std::endl;
    std::cout << "height: " << hei << std::endl;

    mod_exp_anal.write("debiasing_figs_images/cloud_2/exp_anal.png");
    mod_c_1_b_0_25_anal.write("debiasing_figs_images/cloud_2/c_1_0_b_0_25_anal.png");
    mod_c_2_b_0_5_anal.write("debiasing_figs_images/cloud_2/c_2_0_b_0_5_anal.png");
    mod_c_1_5_b_0_75_anal.write("debiasing_figs_images/cloud_2/c_1_5_b_0_75_anal.png");
    mod_exp_deb.write("debiasing_figs_images/cloud_2/exp_deb.png");
    mod_c_1_b_0_25_deb.write("debiasing_figs_images/cloud_2/c_1_0_b_0_25_deb.png");
    mod_c_2_b_0_5_deb.write("debiasing_figs_images/cloud_2/c_2_0_b_0_5_deb.png");
    mod_c_1_5_b_0_75_deb.write("debiasing_figs_images/cloud_2/c_1_5_b_0_75_deb.png");

    // crop_1_deb.write("debiasing_figs_images/cloud_2/deb_crop_1.exr");
    // crop_2_deb.write("debiasing_figs_images/cloud_2/deb_crop_2.exr");
    // crop_3_deb.write("debiasing_figs_images/cloud_2/deb_crop_3.exr");
    // crop_4_deb.write("debiasing_figs_images/cloud_2/deb_crop_4.exr");
    // crop_1_ray.write("debiasing_figs_images/cloud_2/ray_crop_1.exr");
    // crop_2_ray.write("debiasing_figs_images/cloud_2/ray_crop_2.exr");
    // crop_3_ray.write("debiasing_figs_images/cloud_2/ray_crop_3.exr");
    // crop_4_ray.write("debiasing_figs_images/cloud_2/ray_crop_4.exr");

    crop_1_deb.write("debiasing_figs_images/cloud_2/deb_crop_1.png");
    crop_2_deb.write("debiasing_figs_images/cloud_2/deb_crop_2.png");
    crop_3_deb.write("debiasing_figs_images/cloud_2/deb_crop_3.png");
    crop_4_deb.write("debiasing_figs_images/cloud_2/deb_crop_4.png");
    crop_1_ray.write("debiasing_figs_images/cloud_2/ray_crop_1.png");
    crop_2_ray.write("debiasing_figs_images/cloud_2/ray_crop_2.png");
    crop_3_ray.write("debiasing_figs_images/cloud_2/ray_crop_3.png");
    crop_4_ray.write("debiasing_figs_images/cloud_2/ray_crop_4.png");
}

void create_consistent_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/consist/";

    system("mkdir debiasing_figs_images/consistent/");

    imedit::Image scene = imedit::Image(path + "final.exr");
    // imedit::Image fd_ref = imedit::Image(path + "final_fd.exr");
    imedit::Image fd_ref = imedit::Image(path + "consist_8192.exr");
    imedit::Image consist_3 = imedit::Image(path + "consist_8192.exr");
    imedit::Image consist_2 = imedit::Image(path + "consist_256.exr");
    imedit::Image consist_1 = imedit::Image(path + "consist_16.exr");

    imedit::Image mod_fd = imedit::Image(fd_ref.width(), fd_ref.height());
    imedit::Image mod_consist_3 = imedit::Image(consist_3.width(), consist_3.height());
    imedit::Image mod_consist_2 = imedit::Image(consist_2.width(), consist_2.height());
    imedit::Image mod_consist_1 = imedit::Image(consist_1.width(), consist_1.height());

    create_fd_image(fd_ref, mod_fd);
    create_fd_image(consist_3, mod_consist_3);
    create_fd_image(consist_2, mod_consist_2);
    create_fd_image(consist_1, mod_consist_1);

    imedit::Image consist_1_inset = imedit::Image(fd_ref.width()/10, fd_ref.height()/10);
    imedit::Image consist_2_inset = imedit::Image(fd_ref.width()/10, fd_ref.height()/10);
    imedit::Image consist_3_inset = imedit::Image(fd_ref.width()/10, fd_ref.height()/10);

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

    scene.write("debiasing_figs_images/consistent/scene.exr");
    mod_fd.write("debiasing_figs_images/consistent/fd_ref.exr");
    mod_consist_3.write("debiasing_figs_images/consistent/consist_3.exr");
    mod_consist_2.write("debiasing_figs_images/consistent/consist_2.exr");
    mod_consist_1.write("debiasing_figs_images/consistent/consist_1.exr");

    scene.write("debiasing_figs_images/consistent/scene.png");
    mod_fd.write("debiasing_figs_images/consistent/fd_ref.png");
    consist_3_inset.write("debiasing_figs_images/consistent/consist_3.png");
    consist_2_inset.write("debiasing_figs_images/consistent/consist_2.png");
    consist_1_inset.write("debiasing_figs_images/consistent/consist_1.png");
}

void create_shader_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/shader/";

    system("mkdir debiasing_figs_images/shader/");

    imedit::Image scene = imedit::Image(path + "real-final.exr");
    imedit::Image biased = imedit::Image(path + "shader_biased.exr");
    imedit::Image ref = imedit::Image(path + "shader_ref.exr");
    imedit::Image unb = imedit::Image(path + "shader_unb.exr");

    imedit::Image mod_scene = imedit::Image(scene.width(), scene.height());
    imedit::Image mod_biased = imedit::Image(biased.width(), biased.height());
    imedit::Image mod_ref = imedit::Image(ref.width(), ref.height());
    imedit::Image mod_unb = imedit::Image(unb.width(), unb.height());

    create_fd_image(scene, mod_scene);
    create_fd_image(biased, mod_biased);
    create_fd_image(ref, mod_ref);
    create_fd_image(unb, mod_unb);

    scene.write("debiasing_figs_images/shader/scene.exr");
    mod_biased.write("debiasing_figs_images/shader/preexp_biased.exr");
    mod_ref.write("debiasing_figs_images/shader/preexp_ref.exr");
    mod_unb.write("debiasing_figs_images/shader/preexp_unb.exr");

    int i1_offset = 200;
    int j1_offset = 175;
    int i2_offset = 345;
    int j2_offset = 125;

    int wid = 50;
    int hei = 35;

    imedit::Image biased_inset_1 = imedit::Image(wid, hei);
    imedit::Image ref_inset_1 = imedit::Image(wid, hei);
    imedit::Image unb_inset_1 = imedit::Image(wid, hei);

    imedit::Image biased_inset_2 = imedit::Image(wid, hei);
    imedit::Image ref_inset_2 = imedit::Image(wid, hei);
    imedit::Image unb_inset_2 = imedit::Image(wid, hei);

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

    mod_biased.write("debiasing_figs_images/shader/biased.exr");
    mod_ref.write("debiasing_figs_images/shader/ref.exr");
    mod_unb.write("debiasing_figs_images/shader/unb.exr");

    scene.write("debiasing_figs_images/shader/scene.png");
    mod_biased.write("debiasing_figs_images/shader/biased.png");
    mod_ref.write("debiasing_figs_images/shader/ref.png");
    mod_unb.write("debiasing_figs_images/shader/unb.png");

    biased_inset_1.write("debiasing_figs_images/shader/biased_inset_1.png");
    ref_inset_1.write("debiasing_figs_images/shader/ref_inset_1.png");
    unb_inset_1.write("debiasing_figs_images/shader/unb_inset_1.png");
    biased_inset_2.write("debiasing_figs_images/shader/biased_inset_2.png");
    ref_inset_2.write("debiasing_figs_images/shader/ref_inset_2.png");
    unb_inset_2.write("debiasing_figs_images/shader/unb_inset_2.png");

    biased_inset_1.write("debiasing_figs_images/shader/biased_inset_1.exr");
    ref_inset_1.write("debiasing_figs_images/shader/ref_inset_1.exr");
    unb_inset_1.write("debiasing_figs_images/shader/unb_inset_1.exr");
    biased_inset_2.write("debiasing_figs_images/shader/biased_inset_2.exr");
    ref_inset_2.write("debiasing_figs_images/shader/ref_inset_2.exr");
    unb_inset_2.write("debiasing_figs_images/shader/unb_inset_2.exr");
}

void create_teaser_figure()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/teaser/";

    system("mkdir debiasing_figs_images/teaser/");

    imedit::Image teaser = imedit::Image(path + "base_teaser.exr");
    imedit::Image fd = imedit::Image(path + "fd_teaser.exr");

    imedit::Image mod_fd = imedit::Image(fd.width(), fd.height());

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

    imedit::Image combined = imedit::Image(fd.width(), fd.height());

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

    teaser.write("debiasing_figs_images/teaser/teaser_base.png");
    mod_fd.write("debiasing_figs_images/teaser/teaser_fd.png");
    combined.write("debiasing_figs_images/teaser/combined_teaser.png");
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
