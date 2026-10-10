#include <Arduino.h>
#include <Render.h>
#include <Render.cpp>
#include <VertexMaps.h>
#include <VertexMaps.cpp>

Shape2D ProtoEye;
Shape2D ProtoNose;
Shape2D ProtoMouth;
FaceData2D ProtoFace{
  {ProtoEye, ProtoNose, ProtoMouth},
  0,
  {0.0f, 0.0f},
  {1.0f, 1.0f}
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  ProtoFace.Face[0] = ShapeTransform2D(Eye1, 0, {10.0f, 10.0f}, {0.25f, 0.25f});
  ProtoFace.Face[1] = ShapeTransform2D(Nose1, 0, {1.5f, 10.0f}, {0.25f, 0.25f});
  ProtoFace.Face[2] = ShapeTransform2D(Mouth1, 0, {5.5f, 3.0f}, {0.25f, 0.25f});

  SendVertexData(ProtoFace.Face);
  delay(20);
}



