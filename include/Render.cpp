#pragma once
#include <Arduino.h>
#include <Render.h>
#include <VertexMaps.h>

/// @brief Debug Tool, prints vertices in Teleplot.
/// @param FG A passed by value FaceGroup. A vector of shapes and their vertices.
void SendVertexData(const FaceGroup2D& FG){
    for(int i = 0; i < FG.size(); i++){
        for(int j = 0; j < FG[i].Vertices.size(); j++){
            Serial.print(">Vertices:");
            Serial.print(FG[i].Vertices[j][0]);
            Serial.print(":");
            Serial.print(FG[i].Vertices[j][1]);
            Serial.println("|xy");

            Serial.print(">Vertices");
            Serial.print(i);
            Serial.print(":");
            Serial.print(FG[i].Vertices[j][0]);
            Serial.print(":");
            Serial.print(FG[i].Vertices[j][1]);
            Serial.println("|xy");
        }
    }
}

void RasterizeLayer(const FaceData2D& VectorArt, image& PixelArt){
    for(int i = 0; i < VectorArt.Face.size(); i++){  // for each shape
        for(int j = 0; j < VectorArt.Face[i].Buffer.size(); j++){ // for each list of triangle points

           Vertex2D A = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][0]];
           Vertex2D B = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][1]];
           Vertex2D C = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][2]];

        

        }

    } 


}