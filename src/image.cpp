#include <imedit/image.h>
#include <stdexcept>
#include <algorithm>
#include <fstream>

#define STB_IMAGE_STATIC
#define STB_IMAGE_WRITE_STATIC

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#define TINYEXR_IMPLEMENTATION
#include <tinyexr/tinyexr.h>

// due to include stuff regarding stb and tinyexr,
// all image io is defined here for all types

namespace imedit
{
    // The read and write logic is based off of code written by Wojciech Jarosz
    bool image_read(const std::string &filename, RGBImage<float> &image)
    {
        int wid;
        int hei;
        int dep;

        try
        {
            if (getExtension(filename) == "txt")
            {
                std::string line;
                std::ifstream file(filename);

                std::getline(file, line);
                int wid = std::stoi(line);
                std::getline(file, line);
                int hei = std::stoi(line);

                image.resize(wid, hei);

                for (int y = 0; y < image.height(); ++y)
                {
                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int z = 0; z < 3; ++z)
                        {
                            std::getline(file, line);
                            image(x, y, z) = (float)std::stod(line);
                        }
                    }
                }

                return true;
            }
            else if (getExtension(filename) == "exr")
            {
                float *out;
                int tmp_w;
                int tmp_h;
                const char *err = nullptr;

                int ret = LoadEXR(&out, &tmp_w, &tmp_h, filename.c_str(), &err);
                if (ret != TINYEXR_SUCCESS)
                {
                    if (err)
                    {
                        std::cout << err << std::endl;
                        FreeEXRErrorMessage(err);
                    }
                }
                else
                {
                    image.resize(tmp_w, tmp_h);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                image(x, y, z) = (float)out[4 * (x + y * image.width()) + z];
                            }
                        }
                    }

                    free(out);
                    return true;
                }
            }
            else if (stbi_is_hdr(filename.c_str()))
            {
                float *pxls = stbi_loadf(filename.c_str(),
                                         &wid,
                                         &hei,
                                         &dep,
                                         3);
                if (pxls)
                {
                    image.resize(wid, hei);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                image(x, y, z) = (float)pxls[3 * (x + y * image.width()) + z];
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load HDR image");
                }
            }
            else
            {
                unsigned char *pxls = stbi_load(filename.c_str(),
                                                &wid,
                                                &hei,
                                                &dep,
                                                4);

                // std::cout << "DEP: " << dep << std::endl;
                if (pxls)
                {
                    image.resize(wid, hei);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                // change all of this back!
                                // if (pxls[4 * (x + y * w) + 3] != 0.0)
                                byteToVal(pxls[4 * (x + y * image.width()) + z], image(x, y, z));
                                // else
                                //     byteToVal(0.0, operator()(x, y, z));
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load LDR image");
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error reading Image for file: \"" << filename << "\":\n\t"
                      << stbi_failure_reason() << std::endl;
            return false;
        }

        return false;
    }

    bool image_write(const std::string &filename, RGBImage<float> &image)
    {
        if (getExtension(filename) == "txt")
        {
            std::ofstream file(filename);
            file << image.width() << "\n";
            file << image.height() << "\n";

            for (int z = 0; z < 3; ++z)
            {
                for (int x = 0; x < image.width(); ++x)
                {
                    for (int y = 0; y < image.height(); ++y)
                    {
                        file << image(x, y, z) << "\n";
                    }
                }
            }

            file.close();
        }
        else
        {
            std::string extension = getExtension(filename);
            // std::cout << "EXT: " << extension << std::endl;

            std::transform(extension.begin(),
                           extension.end(),
                           extension.begin(),
                           ::tolower);

            try
            {
                if (extension == "hdr")
                {
                    std::vector<float> pxls(image.height() * image.width() * 3, 1.f);
                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int y = 0; y < image.height(); ++y)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                pxls[z + 3 * (x + y * image.width())] = image(x, y, z);
                            }
                        }
                    }

                    if (!stb::stbi_write_hdr(filename.c_str(), image.width(), image.height(), 3, &pxls[0]))
                    {
                        throw std::runtime_error("Could not write HDR image");
                    }
                }
                else if (extension == "exr")
                {
                    EXRHeader header;
                    InitEXRHeader(&header);

                    EXRImage final_image;
                    InitEXRImage(&final_image);

                    final_image.num_channels = 3;

                    std::vector<float> images[3];
                    images[0].resize(image.width() * image.height());
                    images[1].resize(image.width() * image.height());
                    images[2].resize(image.width() * image.height());

                    for (int i = 0; i < image.height(); i++)
                    {
                        for (int j = 0; j < image.width(); ++j)
                        {
                            images[0][i * image.width() + j] = image(j, i).r;
                            images[1][i * image.width() + j] = image(j, i).g;
                            images[2][i * image.width() + j] = image(j, i).b;
                        }
                    }

                    float *image_ptr[3];
                    image_ptr[0] = &(images[2].at(0)); // B
                    image_ptr[1] = &(images[1].at(0)); // G
                    image_ptr[2] = &(images[0].at(0)); // R

                    final_image.images = (unsigned char **)image_ptr;
                    final_image.width = image.width();
                    final_image.height = image.height();

                    header.num_channels = 3;
                    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
                    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
                    strncpy(header.channels[0].name, "B", 255);
                    header.channels[0].name[strlen("B")] = '\0';
                    strncpy(header.channels[1].name, "G", 255);
                    header.channels[1].name[strlen("G")] = '\0';
                    strncpy(header.channels[2].name, "R", 255);
                    header.channels[2].name[strlen("R")] = '\0';

                    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
                    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);

                    for (int i = 0; i < header.num_channels; i++)
                    {
                        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;          // pixel type of input image
                        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
                    }

                    const char *err;
                    int ret = SaveEXRImageToFile(&final_image, &header, filename.c_str(), &err);

                    if (ret != TINYEXR_SUCCESS)
                    {
                        fprintf(stderr, "Save EXR err: %s\n", err);
                        return ret;
                    }

                    free(header.channels);
                    free(header.pixel_types);
                    free(header.requested_pixel_types);

                    return true;
                }
                else if (extension == "png" ||
                         extension == "bmp" ||
                         extension == "tga" ||
                         extension == "jpg" ||
                         extension == "jpeg")
                {
                    int outC = 4;
                    std::vector<unsigned char> pxls(image.height() * image.width() * outC, 255);

                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int y = 0; y < image.height(); ++y)
                        {
                            int z;
                            for (z = 0; z < 3; ++z)
                            {
                                // 2.2 does not exactly match .exr for low pixel values... fix later
                                // pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, z), 1.0 / 2.2));
                                pxls[z + outC * (x + y * image.width())] = valToByte(image(x, y, z));
                            }
                            for (; z < 3; ++z)
                            {
                                // pxls[z + outC * (x + y * w)] = valToByte(pow(operator()(x, y, 0), 1.0 / 2.2));
                                pxls[z + outC * (x + y * image.width())] = valToByte(image(x, y, z));
                            }
                        }
                    }

                    if (extension == "png")
                    {
                        if (!stb::stbi_write_png(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0],
                                                 sizeof(unsigned char) * image.width() * outC))
                        {
                            throw std::runtime_error("Could not write PNG image.");
                        }
                    }
                    else if (extension == "bmp")
                    {
                        if (!stb::stbi_write_bmp(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write BMP image.");
                        }
                    }
                    else if (extension == "tga")
                    {
                        if (!stb::stbi_write_tga(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write TGA image.");
                        }
                    }
                    else if (extension == "jpg" || extension == "jpeg")
                    {
                        if (!stb::stbi_write_jpg(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0], 100))
                        {
                            throw std::runtime_error("Could not write JPEG image.");
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("Invalid extension");
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "ERROR :: Image::write(" << filename << ") :: " << e.what() << std::endl;
            }
        }

        return true;
    }

    bool image_read(const std::string &filename, RGBImage<double> &image)
    {
        int wid;
        int hei;
        int dep;

        try
        {
            if (getExtension(filename) == "txt")
            {
                std::string line;
                std::ifstream file(filename);

                std::getline(file, line);
                int wid = std::stoi(line);
                std::getline(file, line);
                int hei = std::stoi(line);

                image.resize(wid, hei);

                for (int y = 0; y < image.height(); ++y)
                {
                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int z = 0; z < 3; ++z)
                        {
                            std::getline(file, line);
                            image(x, y, z) = (double)std::stod(line);
                        }
                    }
                }

                return true;
            }
            else if (getExtension(filename) == "exr")
            {
                float *out;
                int tmp_w;
                int tmp_h;
                const char *err = nullptr;

                int ret = LoadEXR(&out, &tmp_w, &tmp_h, filename.c_str(), &err);
                if (ret != TINYEXR_SUCCESS)
                {
                    if (err)
                    {
                        std::cout << err << std::endl;
                        FreeEXRErrorMessage(err);
                    }
                }
                else
                {
                    image.resize(tmp_w, tmp_h);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                image(x, y, z) = (double)out[4 * (x + y * image.width()) + z];
                            }
                        }
                    }

                    free(out);
                    return true;
                }
            }
            else if (stbi_is_hdr(filename.c_str()))
            {
                float *pxls = stbi_loadf(filename.c_str(),
                                         &wid,
                                         &hei,
                                         &dep,
                                         3);
                if (pxls)
                {
                    image.resize(wid, hei);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                image(x, y, z) = (double)pxls[3 * (x + y * image.width()) + z];
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load HDR image");
                }
            }
            else
            {
                unsigned char *pxls = stbi_load(filename.c_str(),
                                                &wid,
                                                &hei,
                                                &dep,
                                                4);

                if (pxls)
                {
                    image.resize(wid, hei);

                    for (int y = 0; y < image.height(); ++y)
                    {
                        for (int x = 0; x < image.width(); ++x)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                byteToVal(pxls[4 * (x + y * image.width()) + z], image(x, y, z));
                            }
                        }
                    }

                    stbi_image_free(pxls);
                    return true;
                }
                else
                {
                    throw std::runtime_error("Could not load LDR image");
                }
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error reading Image for file: \"" << filename << "\":\n\t"
                      << stbi_failure_reason() << std::endl;
            return false;
        }

        return false;
    }

    bool image_write(const std::string &filename, RGBImage<double> &image)
    {
        if (getExtension(filename) == "txt")
        {
            std::ofstream file(filename);
            file << image.width() << "\n";
            file << image.height() << "\n";

            for (int z = 0; z < 3; ++z)
            {
                for (int x = 0; x < image.width(); ++x)
                {
                    for (int y = 0; y < image.height(); ++y)
                    {
                        file << image(x, y, z) << "\n";
                    }
                }
            }

            file.close();
        }
        else
        {
            std::string extension = getExtension(filename);

            std::transform(extension.begin(),
                           extension.end(),
                           extension.begin(),
                           ::tolower);

            try
            {
                if (extension == "hdr")
                {
                    std::vector<float> pxls(image.height() * image.width() * 3, 1.f);
                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int y = 0; y < image.height(); ++y)
                        {
                            for (int z = 0; z < 3; ++z)
                            {
                                pxls[z + 3 * (x + y * image.width())] = image(x, y, z);
                            }
                        }
                    }

                    if (!stb::stbi_write_hdr(filename.c_str(), image.width(), image.height(), 3, &pxls[0]))
                    {
                        throw std::runtime_error("Could not write HDR image");
                    }
                }
                else if (extension == "exr")
                {
                    EXRHeader header;
                    InitEXRHeader(&header);

                    EXRImage final_image;
                    InitEXRImage(&final_image);

                    final_image.num_channels = 3;

                    std::vector<float> images[3];
                    images[0].resize(image.width() * image.height());
                    images[1].resize(image.width() * image.height());
                    images[2].resize(image.width() * image.height());

                    for (int i = 0; i < image.height(); i++)
                    {
                        for (int j = 0; j < image.width(); ++j)
                        {
                            images[0][i * image.width() + j] = (float)image(j, i).r;
                            images[1][i * image.width() + j] = (float)image(j, i).g;
                            images[2][i * image.width() + j] = (float)image(j, i).b;
                        }
                    }

                    float *image_ptr[3];
                    image_ptr[0] = &(images[2].at(0)); // B
                    image_ptr[1] = &(images[1].at(0)); // G
                    image_ptr[2] = &(images[0].at(0)); // R

                    final_image.images = (unsigned char **)image_ptr;
                    final_image.width = image.width();
                    final_image.height = image.height();

                    header.num_channels = 3;
                    header.channels = (EXRChannelInfo *)malloc(sizeof(EXRChannelInfo) * header.num_channels);
                    // Must be BGR(A) order, since most of EXR viewers expect this channel order.
                    strncpy(header.channels[0].name, "B", 255);
                    header.channels[0].name[strlen("B")] = '\0';
                    strncpy(header.channels[1].name, "G", 255);
                    header.channels[1].name[strlen("G")] = '\0';
                    strncpy(header.channels[2].name, "R", 255);
                    header.channels[2].name[strlen("R")] = '\0';

                    header.pixel_types = (int *)malloc(sizeof(int) * header.num_channels);
                    header.requested_pixel_types = (int *)malloc(sizeof(int) * header.num_channels);

                    for (int i = 0; i < header.num_channels; i++)
                    {
                        header.pixel_types[i] = TINYEXR_PIXELTYPE_FLOAT;          // pixel type of input image
                        header.requested_pixel_types[i] = TINYEXR_PIXELTYPE_HALF; // pixel type of output image to be stored in .EXR
                    }

                    const char *err;
                    int ret = SaveEXRImageToFile(&final_image, &header, filename.c_str(), &err);

                    if (ret != TINYEXR_SUCCESS)
                    {
                        fprintf(stderr, "Save EXR err: %s\n", err);
                        return ret;
                    }

                    free(header.channels);
                    free(header.pixel_types);
                    free(header.requested_pixel_types);

                    return true;
                }
                else if (extension == "png" ||
                         extension == "bmp" ||
                         extension == "tga" ||
                         extension == "jpg" ||
                         extension == "jpeg")
                {
                    int outC = 4;
                    std::vector<unsigned char> pxls(image.height() * image.width() * outC, 255);

                    for (int x = 0; x < image.width(); ++x)
                    {
                        for (int y = 0; y < image.height(); ++y)
                        {
                            int z;
                            for (z = 0; z < 3; ++z)
                            {
                                pxls[z + outC * (x + y * image.width())] = valToByte(image(x, y, z));
                            }
                            for (; z < 3; ++z)
                            {
                                pxls[z + outC * (x + y * image.width())] = valToByte(image(x, y, z));
                            }
                        }
                    }

                    if (extension == "png")
                    {
                        if (!stb::stbi_write_png(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0],
                                                 sizeof(unsigned char) * image.width() * outC))
                        {
                            throw std::runtime_error("Could not write PNG image.");
                        }
                    }
                    else if (extension == "bmp")
                    {
                        if (!stb::stbi_write_bmp(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write BMP image.");
                        }
                    }
                    else if (extension == "tga")
                    {
                        if (!stb::stbi_write_tga(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0]))
                        {
                            throw std::runtime_error("Could not write TGA image.");
                        }
                    }
                    else if (extension == "jpg" || extension == "jpeg")
                    {
                        if (!stb::stbi_write_jpg(filename.c_str(), image.width(), image.height(),
                                                 outC, &pxls[0], 100))
                        {
                            throw std::runtime_error("Could not write JPEG image.");
                        }
                    }
                    else
                    {
                        throw std::invalid_argument("Invalid extension");
                    }
                }
            }
            catch (const std::exception &e)
            {
                std::cout << "ERROR :: Image::write(" << filename << ") :: " << e.what() << std::endl;
            }
        }

        return true;
    }
}
