#pragma once
#include <VertexMaps.h>

static uint8_t img[DEFAULT_IMAGE_WIDTH][DEFAULT_IMAGE_HEIGHT][3];


//scale
//shake
//distort

//rasterize

//shaders

using RGBA = std::array<float, 4>;

void SendVertexData(const FaceGroup2D& FG);

void RasterizeLayer(const FaceData2D& VectorArt, image& PixelArt);

float LERP(float a, float b, float t);