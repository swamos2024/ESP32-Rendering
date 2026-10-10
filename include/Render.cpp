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
    //this will be a very highly used function with long for loops so every optimization counts.

    for(int i = 0; i < VectorArt.Face.size(); i++){ // for each shape
        for(int j = 0; j < VectorArt.Face[i].Buffer.size(); j++){ // for each triangle (reading from the list of triangle points)

            Vertex2D A = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][0]]; //find and save the triangle vertices.
            Vertex2D B = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][1]];
            Vertex2D C = VectorArt.Face[i].Vertices[VectorArt.Face[i].Buffer[j][2]];

            // use the area to order the vertices in a counter clockwise direction
            float area = (B[0] - A[0]) * (C[1] - A[1]) - (B[1] - A[1]) * (C[0] - A[0]);
            if (area == 0.0f) continue;          // degenerate triangle, nothing to draw
            if (area < 0.0f) {                   // wrong winding, swap B and C
                std::swap(B, C);
                //area = -area;
            }
        
            float minx = std::min({A[0], B[0], C[0]}); // triangle binding box dimensions
            float maxx = std::max({A[0], B[0], C[0]});
            float miny = std::min({A[1], B[1], C[1]});
            float maxy = std::max({A[1], B[1], C[1]});

            // for the next parts we will use IMAGE_WIDTH - 1 and IMAGE_HEIGHT -1 since we need an array of IMAGE_WIDTH by IMAGE_HEIGHT and arrays start at 0.
            // TODO it is likely using IMAGE_WIDTH - 1 and IMAGE_HEIGHT -1 will slow things down slightly, use a new #define if needed.

            // If the a minimum is above a maximum image edge or a maximum is below a minimum frame edge
            // then the whole triangle is outside of frame and can be skiped.
            if (maxx < 0 || minx > IMAGE_WIDTH - 1 ||
            maxy < 0 || miny > IMAGE_HEIGHT - 1)
            continue; 

            // if the binding box is sticking out of frame, crop it into frame. 
            int x0 = std::max(0, (int)std::floor(minx));
            int x1 = std::min(IMAGE_WIDTH - 1, (int)std::ceil(maxx));
            int y0 = std::max(0, (int)std::floor(miny));
            int y1 = std::min(IMAGE_HEIGHT - 1, (int)std::ceil(maxy));

            // this upcoming for loop uses barycentric coordinates to check if a pixel is inside or out of a triangle
            // the vector operation to solve barycentric coordinates is linear so  we only need to solve them in one spot
            // then we can solve the rest by adding the coordinates slope times distance from the initial point. 
            float dAlpha_dx = B[1] - C[1],  dAlpha_dy = C[0] - B[0];
            float dBeta_dx  = C[1] - A[1],  dBeta_dy  = A[0] - C[0];
            float dGamma_dx = A[1] - B[1],  dGamma_dy = B[0] - A[0];
            
            for(int x = minx; x < maxx; x++){
                for(int y = miny; y < maxy; y++){
                    
                    Vertex2D P = {x, y};



                    Vertex2D a = A - P, b = B - P, c = C - P; // Im solving for these so we dont repeat 9 subtractions. 

                    float AlphaCorner = (b)[0]*(c)[1] - (b)[1]*(c)[0];
                    float BetaCorner = (c)[0]*(a)[1] - (c)[1]*(a)[0];
                    float GammaCorner = (a)[0]*(b)[1] - (a)[1]*(b)[0];
                    

 
                }                
            }
        }
    } 
}