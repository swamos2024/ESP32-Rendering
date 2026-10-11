#pragma once
#include <Arduino.h>
#include <Render.h>
#include <VertexMaps.h>
#include <algorithm>
#include <cmath>


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

//TODO I may have used alpha gamma and beta names improperly for this subject as distance, instead of area.
void RasterizeLayer(const FaceData2D& VectorArt, image& PixelArt){
    //this will be a very highly used function with long for loops so every optimization counts.

    for(int i = 0; i < VectorArt.Face.size(); i++){ // for each shape
        for(int j = 0; j < VectorArt.Face[i].Buffer.size(); j++){ // for each triangle (reading from the list of triangle points)

            #pragma region  Triangle Preparation
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

            // if the binding box is sticking out of frame, crop it into frame. also snap it to a whole pixel
            int x0 = std::max(0, (int)std::floor(minx));
            int x1 = std::min(IMAGE_WIDTH - 1, (int)std::ceil(maxx));
            int y0 = std::max(0, (int)std::floor(miny));
            int y1 = std::min(IMAGE_HEIGHT - 1, (int)std::ceil(maxy));
            
            #pragma endregion
            #pragma region Triangle Measuring
            // During this segment, we will scan through the triangles area using barycentric coordinates (alpha, beta, gamma),
            // where alpha is the distance to the BC edge, beta is distance to the AC edge, and gamma is distance to the AB edge.
            // to find them at a point P, we divide the triangle into sub sections with lines PA, PB, PC. and use a vector cross 
            // product to solve area. since A = 0.5*B*H we divide by 0.5 * B to solve for the final number H, where B is the segment
            // lengths AB BC and AC and H is the distance to those segments at P. to optimize we never divide the area by 2, and in 
            // turn dont need to divide by 0.5.

            // the resulting coordinates let us determine if we are inside a triangle, and do anti-aliasing at the same time.

            float AB = Distance(A, B); // solve side lengths to calculate alpha later (transparency, not the coordinate).
            float BC = Distance(B, C);
            float AC = Distance(A, C);

            // the vector operation to solve barycentric coordinates is linear so  we only need to solve them in one spot
            // then we can solve the rest by adding the coordinates slope times distance from the initial point. 
            float dAlpha_dx = (B[1] - C[1]) / BC,  dAlpha_dy = (C[0] - B[0]) / BC;
            float dBeta_dx  = (C[1] - A[1]) / AC,  dBeta_dy  = (A[0] - C[0]) / AC;
            float dGamma_dx = (A[1] - B[1]) / AB,  dGamma_dy = (B[0] - A[0]) / AB;

            //Find the corners coordinate values
            Vertex2D P = {x0 + 0.5f, y0 + 0.5f};
            Vertex2D a = A - P, b = B - P, c = C - P; // Im solving for these so we dont repeat 9 subtractions. 
            float AlphaCorner = ((b)[0]*(c)[1] - (b)[1]*(c)[0]) / BC;
            float BetaCorner = ((c)[0]*(a)[1] - (c)[1]*(a)[0]) / AC;
            float GammaCorner = ((a)[0]*(b)[1] - (a)[1]*(b)[0]) / AB;

            float alpha;
            float beta;
            float gamma;

            // xi and yi count the full pixel distance from our starting position. 
            float xi = 0;
            for(int x = x0; x < x1; x++){ 
                float yi = 0;
                for(int y = y0; y < y1; y++){
                    
                    P = {x + 0.5f, y + 0.5f};

                    alpha = AlphaCorner + (dAlpha_dx * xi) + (dAlpha_dy * yi);
                    beta = BetaCorner + (dBeta_dx * xi) + (dBeta_dy * yi);
                    gamma = GammaCorner + (dGamma_dx * xi) + (dGamma_dy * yi);

                    // if we are outside the area, skip solving opacity
                    if( alpha > 0 && beta > 0 && gamma > 0){
                        //anti-aliasing:
                        // (std::min(std::max(D, 0.0f), 0.5f) + 0.5f), where D is coordinate gives the amount of area covered by the 
                        // triangle assuming we are near only the edge of D. multiplying them together lets us approximate the area.
                        PixelArt[x][y][3] += (std::min(std::max(alpha, -5.0f), 0.5f) + 0.5f) * (std::min(std::max(beta, -5.0f), 0.5f) + 0.5f) * (std::min(std::max(gamma, -5.0f), 0.5f) + 0.5f);
                        PixelArt[x][y][3] = std::max(PixelArt[x][y][3], 1);
                        PixelArt[x][y][0] = 255; //TODO color is a debug currently, fix later.
                        PixelArt[x][y][1] = 160;
                        PixelArt[x][y][2] = 255;
                    }
                    
                    yi ++;
                }
                xi ++;                
            }
            #pragma endregion
        }
    } 
}

float LERP(float a, float b, float t){
    return a + t*(b - a);
}