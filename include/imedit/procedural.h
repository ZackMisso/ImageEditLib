/*
    ImageEditLib was developed by Zack Misso
    <zackary.t.misso.gr@dartmouth.edu>
*/

#include "image.h"

// including z axis for now in case I want to extend this to support procedural
// voxel maps in the future
struct ImagePartition
{
    IP_X, // x axis only
    IP_Y, // y axis only
    IP_Z, // z axis only
    IP_R, // r channel only
    IP_G, // g channel only
    IP_B, // b channel only
    IP_A, // a channel only

    IP_XY, // x and y axis
    IP_XZ, // x and z axis
    IP_YZ, // y and z axis
    IP_XYZ, // all axis

    IP_RG, // r and g channels only
    IP_RB, // r and b channels only
    IP_RA, // r and a channels only
    IP_GB, // g and b channels only
    IP_GA, // g and a channels only
    IP_BA, // b and a channels only

    IP_RGB, // r g, and b channels only
    IP_RGA, // r g, and a channels only
    IP_RBA, // r b, and a channels only
    IP_GBA, // g b, and a channels only
    IP_RGBA // all channels
};

template <typename T>
void sin_image(T amplitude,
               T period,
               Image<T>& image,
               ImagePartition part)
{
    switch(part)
    {
        case IP_X:
        {
            // TODO
            break;
        }
        case IP_Y:
        {
            // TODO
            break;
        }
        case IP_Z:
        {
            // TODO
            break;
        }
        case IP_R:
        {
            // TODO
            break;
        }
        case IP_G:
        {
            // TODO
            break;
        }
        case IP_B:
        {
            // TODO
            break;
        }
        case IP_A:
        {
            // TODO
            break;
        }
        case IP_XY:
        {
            // TODO
            break;
        }
        case IP_XZ:
        {
            // TODO
            break;
        }
        case IP_YZ:
        {
            // TODO
            break;
        }
        case IP_XYZ:
        {
            // TODO
            break;
        }
        case IP_RG:
        {
            // TODO
            break;
        }
        case IP_RB:
        {
            // TODO
            break;
        }
        case IP_RA:
        {
            // TODO
            break;
        }
        case IP_GB:
        {
            // TODO
            break;
        }
        case IP_GA:
        {
            // TODO
            break;
        }
        case IP_BA:
        {
            // TODO
            break;
        }
        case IP_RGB:
        {
            // TODO
            break;
        }
        case IP_RGA:
        {
            // TODO
            break;
        }
        case IP_RBA:
        {
            // TODO
            break;
        }
        case IP_GBA:
        {
            // TODO
            break;
        }
        case IP_RGBA:
        {
            // TODO
            break;
        }
    }
}
