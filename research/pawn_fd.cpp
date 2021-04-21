#include <iostream>
#include <imedit/image.h>
#include <fstream>
#include <sstream>

int main()
{
    imedit::Image pawn_1_0 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_1/volpath.exr");
    imedit::Image pawn_0_0 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_0/volpath.exr");
    imedit::Image pawn_0_5 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_2/volpath.exr");
    imedit::Image pawn_0_25 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_3/volpath.exr");
    imedit::Image pawn_0_125 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_4/volpath.exr");
    imedit::Image pawn_0_0625 = imedit::Image("/Users/corneria/Documents/Research/bias_research/pbrttest/results/pawn_fd/delta_5/volpath.exr");
    // imedit::Image pawn_0_03125 = imedit::image("");

    imedit::Image fd_0 = (pawn_0_0 - pawn_1_0) / 1.0;
    imedit::Image fd_1 = (pawn_0_0 - pawn_0_5) / 0.5;
    imedit::Image fd_2 = (pawn_0_0 - pawn_0_25) / 0.25;
    imedit::Image fd_3 = (pawn_0_0 - pawn_0_125) / 0.125;
    imedit::Image fd_4 = (pawn_0_0 - pawn_0_0625) / 0.0625;

    system("mkdir pawn_fd_results");

    fd_0.write("pawn_fd_results/fd_0.exr");
    fd_1.write("pawn_fd_results/fd_1.exr");
    fd_2.write("pawn_fd_results/fd_2.exr");
    fd_3.write("pawn_fd_results/fd_3.exr");
    fd_4.write("pawn_fd_results/fd_4.exr");
}
