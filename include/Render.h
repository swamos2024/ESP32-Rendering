#pragma once
#include <VertexMaps.h>
#include <algorithm>

static uint8_t img[DEFAULT_IMAGE_WIDTH][DEFAULT_IMAGE_HEIGHT][3];

//The rasterizer will use multi multi sample anti-aliasing. Density is the distance between 
#define MSAA_DENSITY 0.25 // how many steps to take to the next pixel division. divisions multi sample anti-aliasing will use in one pixel. (n divisions -> n^2 samples -> 1/n^2 density )

// interpolate switching shapes
//transform
//rotate
//scale
//shake
//distort

//rasterize

//shaders

void SendVertexData(const FaceGroup2D& FG);