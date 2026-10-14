# ESP32 Renderer (Work in Progress)

### Description

This is the main segment of a larger project that will use sensor data to modify shapes displayed on an LED matrix to display a face. The purpose is for a fun convention costume. This program is in the middle of the program pipeline.

Sensor Data --> Shape Vertex Data --> Rasterizer --> Shaders --> Display

This program will handle the middle 3 segments. (This is a work in progress; so far, only the vertex data handler and Rasterizer are completed)

### Vertex Data Handling

Vertices are saved as 2-item arrays and are grouped together in a shape with a vector of these arrays. To divide the vertices into triangles, there is a separate vector, the index buffer, holding 3-item arrays. These arrays store the index positions of vertices in the vertices vector. Shapes are structs holding a vertices array, an index buffer, and transformation data. The transform data lets values be edited without losing their initial positions. There are also Face groups, which group individual shapes and more transform data. 

### Rasterizer

The Rasterizer converts the vector data of the vertices into pixels. The Rasterizer will loop through each triangle of each shape in each shape group 60 times per second. This means there is a large number of operations happening, so optimization is very important. To start each triangle, the function will draw a bounding box around the triangle snapped to the nearest pixel. If the bounding box is beyond the frame, the function crops it back into frame or removes it if it's too far out of frame. To determine if each pixel is inside the triangle, the function will use barycentric coordinates to measure it. Barycentric coordinates divide the triangle into 3 sub-triangles, where their areas become the coordinates alpha, beta, and gamma. The formula used is A = 1/2 (A x B), where A and B are vectors from the point being checked to the triangle vertices. If any of the three are negative, it indicates a pixel outside the triangle. Barycentric coordinates are found with vector operations; these are computationally expensive, so instead the derivatives of alpha, beta, and gamma are taken with respect to x and y. Since the vector equations are linear, we now only need to take the coordinates with the vectors in one place; then we can find the other coordinates by adding the change in x or y times the derivatives of the three coordinates. 

### Anti-aliasing 

Anti-aliasing is an operation also handled by the rasterizer. Anti-aliasing is the smoothing of pixels near jagged edges by editing opacity. To do this, the distance from an interior triangle edge is taken at every point by dividing the barycentric coordinates by the triangle base length. A = 1/2 B * H --> A/B = 1/2 * H. In order to handle the two, we simply never divide by two during the initial vector operations (A/B = 1/2 * H --> 2A/B = H); we never needed the real area for anything, only the sign, so it never mattered. Now H is alone in units of pixels. By clamping the height between 0 and 1 and multiplying opacity by it, the shape gets softened near the edges and removes our jagged edges.
