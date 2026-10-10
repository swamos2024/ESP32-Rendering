#pragma once
#include <cstdint>
#include <array>
#include <vector>

// the default image size is odd to match coela cant's led boards. [34:23]
#define DEFAULT_IMAGE_WIDTH 34
#define DEFAULT_IMAGE_HEIGHT 23

#define IMAGE_WIDTH 34
#define IMAGE_HEIGHT 23
//TODO make sure more regular aspect ratios work.

typedef int image[IMAGE_WIDTH][IMAGE_HEIGHT][4]; //last array dimension is for RGBA.

using Vertex2D = std::array<float, 2>;
using VertexGroup2D = std::vector<Vertex2D>;


using IndexBufferSegment = std::array<int, 3>;
using IndexBuffer = std::vector<IndexBufferSegment>;

struct Shape2D
{
    //These are used to render the initial shape
    VertexGroup2D Vertices;
    IndexBuffer Buffer;

    // these track any changes done
    float Angle;
    Vertex2D Translate;
    Vertex2D Scale;

};

using FaceGroup2D = std::vector<Shape2D>;

struct FaceData2D
{
    FaceGroup2D Face;

    float Angle;
    Vertex2D Translate;
    Vertex2D Scale;
};

#pragma region Vertex Operator Overloads
    inline Vertex2D operator+(const Vertex2D& a, const Vertex2D& b);
    inline Vertex2D operator-(const Vertex2D& a, const Vertex2D& b);
    inline Vertex2D operator*(const Vertex2D& a, const Vertex2D& b); // component-wise
    inline Vertex2D operator/(const Vertex2D& a, const Vertex2D& b); // component-wise

    // compound assignment
    inline Vertex2D& operator+=(Vertex2D& a, const Vertex2D& b);
    inline Vertex2D& operator-=(Vertex2D& a, const Vertex2D& b);
    inline Vertex2D& operator*=(Vertex2D& a, const Vertex2D& b);// component-wise
    inline Vertex2D& operator/=(Vertex2D& a, const Vertex2D& b);// component-wise

    // scalar versions
    inline Vertex2D operator*(const Vertex2D& a, float s);
    inline Vertex2D operator*(float s, const Vertex2D& a);
    inline Vertex2D operator/(const Vertex2D& a, float s);
    inline Vertex2D& operator*=(Vertex2D& a, float s);
    inline Vertex2D& operator/=(Vertex2D& a, float s);
#pragma endregion 

#pragma region Vertex Math Operations
    Shape2D ShapeTransform2D(Shape2D Shape, float Angle, Vertex2D Translate, Vertex2D Scale);

    FaceData2D FaceDataTransform2D(FaceData2D Shape, float Angle, Vertex2D Translate, Vertex2D Scale);
#pragma endregion

#pragma region Test Shapes
Shape2D Square0{
    {{-1.0f, -1.0f},{-1.0f, 1.0f},{1.0f, 1.0f},{1.0f, -1.0f}},
    {{0, 1, 2},{2, 1, 3}},
    0,
    {0.0f, 0.0f},
    {1.0f, 1.0f}
};

Shape2D Square1{
    {{-1.0f, 2.0f},{-1.0f, 4.0f},{1.0f, 4.0f},{1.0f, 2.0f}},
    {{0, 1, 2},{2, 1, 3}},
    0,
    {0.0f, 3.0f},
    {1.0f, 1.0f}
};

FaceData2D TestFace{
    {{Square0},{Square1}},
    0,
    {0.0f, 0.0f},
    {1.0f, 1.0f}
};
#pragma endregion

#pragma region Faces
    Shape2D Eye1{
        {{10.0f, 5.0f},{1.0f, 5.0f},{-5.0f, 4.0f},{-9.0f, 1.0f},{-10.0f, -5.0f},{-1.0f, -5.0f},{5.0f, -4.0f},{9.0f, -1.0f}},
        {{0, 1, 7},{1, 2, 7},{2, 6, 7},{2, 3, 6},{3, 6, 5},{3, 4, 5}},
        0,
        {0.0f, 0.0f},
        {1.0f, 1.0f}
    };

        Shape2D Nose1{
        {{6.0f, 4.0f}, {-5.0f, 2.0f}, {-6.0f, -4.0f}, {-2.0f, -4.0f}, {-2.0f, 0.0f}, {6.0f, 0.0f}},
        {{0, 1, 4}, {1, 2, 4}, {2, 3, 4}, {0, 4, 5}},
        0,
        {0.0f, 3.0f},
        {1.0f, 1.0f}
    };

    Shape2D Mouth1{
        {{16.0f, 2.0f}, {11.0f, 5.0f}, {9.0f, 2.0f}, {5.0f, 0.0f}, {-2.0f, 2.0f}, {-8.0f, 0.0f}, {-14.0f, 2.0f}, {-14.0f, 0.0f}, {-6.0f, -3.0f}, {-2.0f, 0.0f}, {6.0f, -3.0f}, {11.0f, 2.0f}},
        {{0, 1, 2}, {2, 10, 11}, {2, 3, 10}, {3, 9, 10}, {3, 4, 9}, {4, 5, 9}, {5, 8, 9}, {5, 7, 8}, {5, 6, 7}},
        0,
        {0.0f, 3.0f},
        {1.0f, 1.0f}
    };      

    FaceData2D Face1{
        {Eye1, Nose1, Mouth1},
        0,
        {0.0f, 0.0f},
        {1.0f, 1.0f}
    };

#pragma endregion