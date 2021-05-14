#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

void create_didactic_figs()
{
    std::string path = "/Users/corneria/Documents/Research/bias_research/pbrttest/results/final_paper/didactic/";

    system("mkdir debiasing_figs/didactic/");

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

    gt.write("debiasing_figs/didactic/gt.png");
    delta_1.write("debiasing_figs/didactic/delta_1.png");
    delta_2.write("debiasing_figs/didactic/delta_2.png");
    delta_3.write("debiasing_figs/didactic/delta_3.png");
    delta_4.write("debiasing_figs/didactic/delta_4.png");
    delta_5.write("debiasing_figs/didactic/delta_5.png");
    delta_6.write("debiasing_figs/didactic/delta_6.png");
    delta_7.write("debiasing_figs/didactic/delta_7.png");
}

int main()
{
    system("rm -rf debiasing_figs/");
    system("mkdir debiasing_figs/");

    create_didactic_figs();
}
