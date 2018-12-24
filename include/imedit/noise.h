/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>

    All code here is free to use as long as
    proper credit is given to the author
*/

#pragma once

// #############################################
// The following code is modified from a similar implementation in PBRT by Matt Pharr et. al. //

static int NoiseTableSize = 256;

static int NoiseTable[512] = {
    151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140,
    36, 103, 30, 69, 142,
    // Remainder of the noise permutation table
    8, 99, 37, 240, 21, 10, 23, 190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62,
    94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33, 88, 237, 149, 56, 87, 174,
    20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166, 77,
    146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55,
    46, 245, 40, 244, 102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76,
    132, 187, 208, 89, 18, 169, 200, 196, 135, 130, 116, 188, 159, 86, 164, 100,
    109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123, 5, 202, 38, 147,
    118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28,
    42, 223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101,
    155, 167, 43, 172, 9, 129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232,
    178, 185, 112, 104, 218, 246, 97, 228, 251, 34, 242, 193, 238, 210, 144, 12,
    191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107, 49, 192, 214, 31,
    181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
    138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66,
    215, 61, 156, 180, 151, 160, 137, 91, 90, 15, 131, 13, 201, 95, 96, 53, 194,
    233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23, 190, 6,
    148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32,
    57, 177, 33, 88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74,
    165, 71, 134, 139, 48, 27, 166, 77, 146, 158, 231, 83, 111, 229, 122, 60,
    211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244, 102, 143, 54, 65, 25,
    63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196, 135,
    130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226,
    250, 124, 123, 5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59,
    227, 47, 16, 58, 17, 182, 189, 28, 42, 223, 183, 170, 213, 119, 248, 152, 2,
    44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9, 129, 22, 39, 253, 19,
    98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228, 251,
    34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249,
    14, 239, 107, 49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115,
    121, 50, 45, 127, 4, 150, 254, 138, 236, 205, 93, 222, 114, 67, 29, 24, 72,
    243, 141, 128, 195, 78, 66, 215, 61, 156, 180
};

template <typename T>
T lerp(T t, T one, T two)
{
    return (1.0 - t) * one + t * two;
}

template <typename T>
T noiseW(T t)
{
    T one = t * t * t;
    T two = one * t;
    return 6.0 * two * t - 15.0 * two + 10.0 * one;
}

template <typename T>
T gradient(int x, int y, int z, T dx, T dy, T dz) {
    int val = NoiseTable[NoiseTable[NoiseTable[x] + y] + z];

    val &= 15;

    T u = val < 8 || val == 12 || val == 13 ? dx : dy;
    T v = val < 4 || val == 12 || val == 13 ? dy : dz;
    return ((val & 1) ? -u : u) + ((val & 2) ? -v : v);
}

template <typename T>
T pbrtPerlinNoise(T x, T y, T z)
{
    int xIndex = floor(x);
    int yIndex = floor(y);
    int zIndex = floor(z);

    T dx = x - xIndex;
    T dy = y - yIndex;
    T dz = z - zIndex;

    xIndex = xIndex & NoiseTableSize - 1;
    yIndex = yIndex & NoiseTableSize - 1;
    zIndex = zIndex & NoiseTableSize - 1;

    int xip1 = xIndex + 1;
    int yip1 = yIndex + 1;
    int zip1 = zIndex + 1;

    T dxm1 = dx - 1;
    T dym1 = dy - 1;
    T dzm1 = dz - 1;

    T g100 = gradient<T>(xip1, yIndex, zIndex, dxm1, dy, dz);
    T g010 = gradient<T>(xIndex, yip1, zIndex, dx, dym1, dz);
    T g001 = gradient<T>(xIndex, yIndex, zip1, dx, dy, dym1);
    T g110 = gradient<T>(xip1, yip1, zIndex, dxm1, dym1, dz);
    T g101 = gradient<T>(xip1, yIndex, zip1, dxm1, dy, dzm1);
    T g011 = gradient<T>(xIndex, yip1, zip1, dx, dym1, dzm1);
    T g111 = gradient<T>(xip1, yip1, zip1, dxm1, dym1, dzm1);
    T g000 = gradient<T>(xIndex, yIndex, zIndex, dx, dy, dz);

    T nwx = noiseW<T>(dx);
    T nwy = noiseW<T>(dy);
    T nwz = noiseW<T>(dz);

    // trilinear interpolation
    T x00 = lerp(nwx, g000, g100);
    T x10 = lerp(nwx, g010, g110);
    T x01 = lerp(nwx, g001, g101);
    T x11 = lerp(nwx, g011, g111);
    T y0 = lerp(nwy, x00, x10);
    T y1 = lerp(nwy, x01, x11);

    return lerp(nwz, y0, y1);
}

// #############################################

template <typename T>
T noise(T x, T y, T z, T period)
{
    // TODO: experiment with new versions of noise
    return pbrtPerlinNoise<T>(x / period, y / period, z / period);
}

template <typename T>
T turbulence(T x, T y, T z, T period)
{
    T val = 0.0;
    T scale = period;
    // T pixelSize = 1.0 / period;
    T totalScale = 0.0;

    while (scale > 1.0)
    {
        // x /= scale;
        // y /= scale;
        // z /= scale;

        val = val + (noise<T>(x, y, z, scale)) * scale;
        // totalScale += scale;
        scale /= 2.0;
    }

    // if ( val * 0.5 / period < 0.0) std::cout <<  val * 0.5 / period << std::endl;

    return (0.5 + val * 0.5 / period) * 0.5;
}
