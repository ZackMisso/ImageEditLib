#pragma once

#include <unordered_map>
#include <imedit/image.h>

#define FAST_MODE false

// fast mode removes most excess datatypes to make the image comparison
// suitable for more of a production implementation.

namespace imedit
{
    enum ErrorType {
        ERR_NONE = 0x0,
        ERROR = 0x1,
        ABS_ERROR = 0x01,
        REL_ERROR = 0x001,
        SQR_ERROR = 0x0001,
        VARIANCE = 0x00001, // MSE and Variance are the same
        MEAN_ABS_ERROR = 0x000001
    };

    enum OutputType {
        OUT_NONE = 0x0,
        DIFF = 0x1,
        ABS_DIFF = 0x01,
        REL_DIFF = 0x001,
        SQR_DIFF = 0x0001
    };

    template <typename T, typename F>
    struct ImageCompareData {
        // default comparator does absolutely nothing...
        ImageCompareData(const T* image_one, const T* image_two) 
        : image_one(image_one), image_two(image_two), errors_to_report(0x0), outputs_to_report(0x0) {
            initialize_maps();
        }

        ImageCompareData(const T* image_one, const T* image_two, int errors)
        : image_one(image_one), image_two(image_two), errors_to_report(errors), outputs_to_report(0x0) {
            initialize_maps();
        }

        ImageCompareData(const T* image_one, const T* image_two, int errors, int outputs)
        : image_one(image_one), image_two(image_two), errors_to_report(errors), outputs_to_report(outputs) {
            initialize_maps();
        }

        void initialize_maps() {
            #if FAST_MODE
            #else
                outputs = std::unordered_map<OutputType, T>();
                errors = std::unordered_map<ErrorType, F>();

                // initialize the individual images
                // TODO
            #endif
        }

        ~ImageCompareData() {
            // this datastructure does not own the images
            image_one = nullptr;
            image_two = nullptr;
        }

#if FAST_MODE
        F variance;
#else
        std::unordered_map<OutputType, T> outputs;
        std::unordered_map<ErrorType, F> errors;
#endif

        // flags
        const T* image_one;
        const T* image_two;

        int errors_to_report;
        int outputs_to_report;
    };

    template <typename T, typename F>
    void compare_images(ImageCompareData<T,F>& data) {
        // this function always computes: image_two - image_one
        // where image_two is assumed to be the ground truth
#if FAST_MODE
        // TODO
#else
        if (data.errors & ErrorType::ERROR) {
            // TODO
        }

        if (data.errors_to_report & ErrorType::ABS_ERROR) {
            // TODO
        }

        if (data.errors_to_report & ErrorType::REL_ERROR) {
            // TODO
        }

        if (data.errors_to_report & ErrorType::SQR_ERROR) {
            // TODO
        }

        if (data.errors_to_report & ErrorType::VARIANCE) {
            // TODO
        }

        if (data.errors_to_report & ErrorType::MEAN_ABS_ERROR) {
            // TODO
        }

        if (data.outputs_to_report & OutputType::DIFF) {
            // TODO
        }

        if (data.outputs_to_report & OutputType::ABS_DIFF) {
            // TODO
        }

        if (data.outputs_to_report & OutputType::REL_DIFF) {
            // TODO
        }

        if (data.outputs_to_report & OutputType::SQR_DIFF) {
            // TODO
        }
#endif
    }
}