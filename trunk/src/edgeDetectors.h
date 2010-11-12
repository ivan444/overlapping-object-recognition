#ifndef _EDGE_DETECTORS_H
#define _EDGE_DETECTORS_H

#define WHITE 255
#define BLACK 0
#define OUT_OF_IMAGE -1
#define RED_F 0.3
#define GREEN_F 0.59
#define BLUE_F 0.11
#include "image.h"
#include <iostream>
#include <cmath>
using namespace std;

class EdgeDetector{

public:
 GrayImage* binaryConvolution(GrayImage *img);
 GrayImage* boundaryFolower(GrayImage *img);
 GrayImage* sobel(ColorImage* img, int treshold);

private:
 void drawBoundary(GrayImage *origImg, GrayImage *boundImg, 
 				PixelCoordinates moon, PixelCoordinates planet);

};

#endif

