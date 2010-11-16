#ifndef _PAINT_H
#define _PAINT_H

#include "image.h"
#include "edgeSegmentator.h"
#include <vector>

using namespace std;

ColorImage* annotate(GrayImage *gImg, vector<PixelCoordinates> &points);

ColorImage* annotate(GrayImage *gImg, vector<EdgeSegment> &lines);


/*class annotator
{
public:
	annotator(){}
	~annotator(){}
	ColorImage* add(GrayImage *gImg, vector<PixelCoordinates> &points);
	ColorImage* add(GrayImage *gImg, vector<EdgeSegment> &lines);
	//... 
}*/

#endif