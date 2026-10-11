#pragma once
#include <cstdint>
#include <array>
#include <vector>
#include <VertexMaps.h>
#include <cmath>

#pragma region Vertex Operator Overloads
    inline Vertex2D operator+(const Vertex2D& a, const Vertex2D& b) { return {a[0] + b[0], a[1] + b[1]}; }
    inline Vertex2D operator-(const Vertex2D& a, const Vertex2D& b) { return {a[0] - b[0], a[1] - b[1]}; }
    inline Vertex2D operator*(const Vertex2D& a, const Vertex2D& b) { return {a[0] * b[0], a[1] * b[1]}; }  // component-wise
    inline Vertex2D operator/(const Vertex2D& a, const Vertex2D& b) { return {a[0] / b[0], a[1] / b[1]}; }  // component-wise

    // compound assignment
    inline Vertex2D& operator+=(Vertex2D& a, const Vertex2D& b) { a[0] += b[0]; a[1] += b[1]; return a; }
    inline Vertex2D& operator-=(Vertex2D& a, const Vertex2D& b) { a[0] -= b[0]; a[1] -= b[1]; return a; }
    inline Vertex2D& operator*=(Vertex2D& a, const Vertex2D& b) { a[0] *= b[0]; a[1] *= b[1]; return a; }// component-wise
    inline Vertex2D& operator/=(Vertex2D& a, const Vertex2D& b) { a[0] /= b[0]; a[1] /= b[1]; return a; }// component-wise

    // scalar versions
    inline Vertex2D operator*(const Vertex2D& a, float s) { return {a[0] * s, a[1] * s}; }
    inline Vertex2D operator*(float s, const Vertex2D& a) { return a * s; }
    inline Vertex2D operator/(const Vertex2D& a, float s) { return {a[0] / s, a[1] / s}; }
    inline Vertex2D& operator*=(Vertex2D& a, float s)     { a[0] *= s; a[1] *= s; return a; }
    inline Vertex2D& operator/=(Vertex2D& a, float s)     { a[0] /= s; a[1] /= s; return a; }
#pragma endregion

#pragma region Vertex Math Operations

    inline float Distance(Vertex2D a, Vertex2D b){
       float x = (a[0] - b[0]);
       float y = (a[1] - b[1]);
       return std::sqrt( x*x + y*y );
    }

    Shape2D ShapeTransform2D(Shape2D Shape, float Angle, Vertex2D Translate, Vertex2D Scale){
        Shape.Angle = Angle;
        Shape.Translate = Translate;
        Shape.Scale = Scale;

        float SxCosP = Shape.Scale[0] * cos(Shape.Angle);
        float SySinP = Shape.Scale[1] * sin(Shape.Angle);
        float Neg_SxSinP = Shape.Scale[0] * sin(Shape.Angle);
        float SyCosP = Shape.Scale[1] * cos(Shape.Angle);

        for (int i = 0; i < Shape.Vertices.size(); i++){
            Shape.Vertices[i][0] = (Shape.Vertices[i][0] * SxCosP) + (Shape.Vertices[i][1] * SySinP) + Shape.Translate[0];
            Shape.Vertices[i][1] = (Shape.Vertices[i][1] * Neg_SxSinP) + (Shape.Vertices[i][1] * SyCosP) + Shape.Translate[1];
        }

        return Shape;
    }

    FaceData2D FaceDataTransform2D(FaceData2D Face, float Angle, Vertex2D Translate, Vertex2D Scale){
        Face.Angle = Angle;
        Face.Translate = Translate;
        Face.Scale = Scale;

        float SxCosP = Face.Scale[0] * cos(Face.Angle);
        float SySinP = Face.Scale[1] * sin(Face.Angle);
        float Neg_SxSinP = Face.Scale[0] * sin(Face.Angle);
        float SyCosP = Face.Scale[1] * cos(Face.Angle);

        for(int i = 0; i < Face.Face.size(); i++){
            for(int j = 0; j < Face.Face[i].Vertices.size(); j++){
                Face.Face[i].Vertices[j][0] = (Face.Face[i].Vertices[j][0] * SxCosP) + (Face.Face[i].Vertices[j][1] * SySinP) + Face.Face[i].Translate[0];
                Face.Face[i].Vertices[j][1] = (Face.Face[i].Vertices[j][0] * Neg_SxSinP) + (Face.Face[i].Vertices[j][1] * SyCosP) + Face.Face[i].Translate[1];
            }
        }

        return Face;
    }

#pragma endregion

    // void ShapeTransform2D_inplace(Shape2D& Shape, float Angle, Vertex2D Translate, Vertex2D Scale){

    //     float SxCosP = (1/ Shape.Scale[0]) * cos(-Shape.Angle);
    //     float SySinP = (1 /Shape.Scale[1]) * sin(-Shape.Angle);
    //     float Neg_SxSinP = (1 / Shape.Scale[0]) * sin(-Shape.Angle);
    //     float SyCosP = (1 / Shape.Scale[1]) * cos(-Shape.Angle);


    //     for (int i = 0; i < Shape.Vertices.size(); i++){
    //         Shape.Vertices[i][0] = (Shape.Vertices[i][0] * SxCosP) + (Shape.Vertices[i][1] * SySinP) - Shape.Translate[0];
    //         Shape.Vertices[i][0] = (Shape.Vertices[i][0] * Neg_SxSinP) + (Shape.Vertices[i][1] * SyCosP) - Shape.Translate[1];
    //     }

    //     Shape.Angle += Angle;
    //     Shape.Translate += Translate;
    //     Shape.Scale += Scale;

    //     SxCosP = Shape.Scale[0] * cos(Shape.Angle);
    //     SySinP = Shape.Scale[1] * sin(Shape.Angle);
    //     Neg_SxSinP = Shape.Scale[0] * sin(Shape.Angle);
    //     SyCosP = Shape.Scale[1] * cos(Shape.Angle);

    //     for (int i = 0; i < Shape.Vertices.size(); i++){
    //         Shape.Vertices[i][0] = (Shape.Vertices[i][0] * SxCosP) + (Shape.Vertices[i][1] * SySinP) + Shape.Translate[0];
    //         Shape.Vertices[i][0] = (Shape.Vertices[i][0] * Neg_SxSinP) + (Shape.Vertices[i][1] * SyCosP) + Shape.Translate[1];
    //     }
    // }



    // void FaceDataTransform2D_inplace(FaceData2D& Face, float Angle, Vertex2D Translate, Vertex2D Scale){

    //     float SxCosP = (1/ Face.Scale[0]) * cos(-Face.Angle);
    //     float SySinP = (1 /Face.Scale[1]) * sin(-Face.Angle);
    //     float Neg_SxSinP = (1 / Face.Scale[0]) * sin(-Face.Angle);
    //     float SyCosP = (1 / Face.Scale[1]) * cos(-Face.Angle);
    // }
