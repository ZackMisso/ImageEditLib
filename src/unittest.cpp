#include <iostream>

#include <imedit/comparator.h>

struct UnitTest {
    UnitTest(const std::string& path, const std::string& ext)
        : path(path), ext(ext), err_threshold(0.01) {}
    
    UnitTest(const std::string& path, const std::string& ext, double thresh)
        : path(path), ext(ext), err_threshold(thresh) {}

    virtual bool generate_ground_truth() const = 0;
    virtual bool run_test() const = 0;
    virtual std::string name() const = 0;

    std::string path;
    std::string ext;
    double err_threshold;
};

struct RGBToHSLAndBackTest : public UnitTest {
    virtual bool generate_ground_truth() {
        return false;
    }

    virtual bool run_test() const {
        return false;
    }

    virtual std::string name() const { return "RGBToHSLAndBack"; }
};

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

    return true;
}

int main(int argc, char* argv[]) {
    std::cout << "starting imedit unit tests" << std::endl;

    system("rm -rf unit_tests/");
    system("mkdir unit_tests/");

    if (!test_comparator()) {
        std::cout << "test failed" << std::endl;
    }

    return 0;
}