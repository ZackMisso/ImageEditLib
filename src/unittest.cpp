#include <iostream>

#include <imedit/comparator.h>

void init_grad_image(imedit::RGBImage<double>& image) {
    for (int i = 0; i < image.height(); ++i) {
        for (int j = 0; j < image.width(); ++j) {
            image(j, i) = imedit::Pixel<double>(double(j) / double(image.width()),
                                                double(i) / double(image.height()),
                                                std::max(double(j) / double(image.width()),
                                                        double(i) / double(image.height())));
        }
    }
}

bool test_comparator() {
    imedit::RGBImage<double> image = imedit::RGBImage<double>(100, 100);

    init_grad_image(image);

    imedit::write_image("unit_tests/grad_image.exr", image);
}

int main(int argc, char* argv[]) {
    std::cout << "starting imedit unit tests" << std::endl;

    system("rm -rf unit_tests/");
    system("mkdir unit_tests/");

    test_comparator();
    // TODO

    return 0;
}