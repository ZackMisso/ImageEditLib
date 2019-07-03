#pragma once

// Note: this file is being included because tinyexr, a header-only dependency
// required for writing .exr files, is not properly created for integration with
// a templated image class. If the dependencies in image.h are included in more
// than one file it results in duplication errors.

// TODO: REWRITE ALL OF THIS LIBRARY TO NOT BE TEMPLATED

// #include <vector>
//
// namespace imedit
// {
//
// static std::vector<double> read_pixels_from_file(const std::string& filename,
//                                                  int& width,
//                                                  int& height,
//                                                  int& depth);
//
// static void write_pixels_to_file(const std::string& filename,
//                                  const std::vector<double>& pixels,
//                                  int width,
//                                  int height,
//                                  int depth);
//
// static void write_pixels_to_file(const std::string& filename,
//                                  const std::vector<float>& pixels,
//                                  int width,
//                                  int height,
//                                  int depth);
//
// static void write_pixels_to_file(const std::string& filename,
//                                  const std::vector<int>& pixels,
//                                  int width,
//                                  int height,
//                                  int depth);
//
// }
