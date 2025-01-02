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
        // REL_ERROR = 0x001,
        SQR_ERROR = 0x0001,
        // VARIANCE = 0x00001, // MSE and Variance are the same
        MEAN_ABS_ERROR = 0x000001,
        MEAN_SQR_ERROR = 0x0000001,
        MEAN_REL_ERROR = 0x00000001,
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

        ~ImageCompareData() {
            // this datastructure does not own the images
            image_one = nullptr;
            image_two = nullptr;
        }

        void initialize_maps() {
            #if FAST_MODE
            #else
                outputs = std::unordered_map<OutputType, T>();
                errors = std::unordered_map<ErrorType, F>();

                // initialize the individual images
                if (errors & ErrorType::ERROR) {
                    errors[ErrorType::ERROR] = (F)0.0;
                }

                if (errors_to_report & ErrorType::ABS_ERROR) {
                    errors[ErrorType::ABS_ERROR] = (F)0.0;
                }

                // if (data.errors_to_report & ErrorType::REL_ERROR) {
                //     // TODO: maybe
                // }

                if (errors_to_report & ErrorType::SQR_ERROR) {
                    errors[ErrorType::SQR_ERROR] = (F)0.0;
                }

                // if (data.errors_to_report & ErrorType::VARIANCE) {
                //     // TODO: maybe (same as MSE)
                // }

                if (errors_to_report & ErrorType::MEAN_ABS_ERROR) {
                    errors[ErrorType::MEAN_ABS_ERROR] = (F)0.0;
                }

                if (errors_to_report & ErrorType::MEAN_SQR_ERROR) {
                    errors[ErrorType::MEAN_SQR_ERROR] = (F)0.0;
                }

                if (errors_to_report & ErrorType::MEAN_REL_ERROR) {
                    errors[ErrorType::MEAN_REL_ERROR] = (F)0.0;
                }

                if (outputs_to_report & OutputType::DIFF) {
                    outputs[OutputType::DIFF] = T(image_one->width(), image_one->height());
                }

                if (outputs_to_report & OutputType::ABS_DIFF) {
                    outputs[OutputType::ABS_DIFF] = T(image_one->width(), image_one->height());
                }

                if (outputs_to_report & OutputType::REL_DIFF) {
                    outputs[OutputType::REL_DIFF] = T(image_one->width(), image_one->height());
                }

                if (outputs_to_report & OutputType::SQR_DIFF) {
                    outputs[OutputType::SQR_DIFF] = T(image_one->width(), image_one->height());
                }
            #endif
        }

        F& operator()(ErrorType err_type) {
            return errors[err_type];
        }

        T& operator()(OutputType out_type) {
            return outputs[out_type];
        }
        
        F operator[](ErrorType err_type) const {
            return errors[err_type];
        }

        // to avoid inefficient code, leaving this commented for now
        // T operator[](OutputType out_type) const {
        //     return outputs[out_type];
        // }
#if FAST_MODE
        F mse;
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
        mse = compute_mean_sqr_error(*data.one, *data.two);
#else
        if (data.errors & ErrorType::ERROR) {
            data[ErrorType::ERROR] = compute_error(*data.one, *data.two);
        }

        if (data.errors_to_report & ErrorType::ABS_ERROR) {
            data[ErrorType::ABS_ERROR] = compute_abs_error(*data.one, *data.two);
        }

        // if (data.errors_to_report & ErrorType::REL_ERROR) {
        //     // TODO: maybe
        // }

        if (data.errors_to_report & ErrorType::SQR_ERROR) {
            data[ErrorType::SQR_ERROR] = compute_sqr_error(*data.one, *data.two);
        }

        // if (data.errors_to_report & ErrorType::VARIANCE) {
        //     // TODO: maybe (same as MSE)
        // }

        if (data.errors_to_report & ErrorType::MEAN_ABS_ERROR) {
            data[ErrorType::MEAN_ABS_ERROR] = compute_mean_abs_error(*data.one, *data.two);
        }

        if (data.errors_to_report & ErrorType::MEAN_SQR_ERROR) {
            data[ErrorType::MEAN_SQR_ERROR] = compute_mean_sqr_error(*data.one, *data.two);
        }

        if (data.errors_to_report & ErrorType::MEAN_REL_ERROR) {
            data[ErrorType::MEAN_REL_ERROR] = compute_mean_rel_error(*data.one, *data.two);
        }

        if (data.outputs_to_report & OutputType::DIFF) {
            data[OutputType::DIFF] = *data.two - *data.one;
        }

        if (data.outputs_to_report & OutputType::ABS_DIFF) {
            data[OutputType::ABS_DIFF] = *data.two - *data.one;
            data[OutputType::ABS_DIFF].abs();
        }

        if (data.outputs_to_report & OutputType::REL_DIFF) {
            data[OutputType::REL_DIFF] = (*data.two - *data.one);
            data[OutputType::REL_DIFF].abs();
            data[OutputType::REL_DIFF] /= *data.one;
        }

        if (data.outputs_to_report & OutputType::SQR_DIFF) {
            data[OutputType::SQR_DIFF] = (*data.two - *data.one);
            data[OutputType::SQR_DIFF].square();
        }
#endif
    }
}