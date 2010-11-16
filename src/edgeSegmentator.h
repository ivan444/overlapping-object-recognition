#ifndef _EDGE_SEGMENT_H
#define _EDGE_SEGMENT_H

#include <vector>
#include "image.h"
#include <cmath>
using namespace std;


class EdgeSegment
{
private:
	double angle;
	double length;
	double middleX;
	double middleY;
	PixelCoordinates first;
	PixelCoordinates last;
public:
	EdgeSegment(PixelCoordinates _first, PixelCoordinates _last){
		first = _first;
		last = _last;
		middleX = (first.x+last.x)*0.5;
		middleY = (first.y+last.y)*0.5;
		double dx = ((int)(last.x)) - ((int)first.x);
		double dy = ((int)last.y) - ((int)first.y);
		length = sqrt(dx*dx + dy*dy);
		angle = atan2(dy,dx);
	}

	~EdgeSegment(){}
	double getAngle(){return angle;}
	double getLength(){return length;}
	double getMiddleX (){return middleX;}
	double getMiddleY() {return middleY;}
	PixelCoordinates getFirst() {return first;}
	PixelCoordinates getLast() {return last;}
};

class EdgeSegmentator
{
	
public:
	EdgeSegmentator();
	~EdgeSegmentator();
	vector<EdgeSegment> extractFeatures (GrayImage *src, double threshold);
private:
	vector<EdgeSegment> divideAndConquer( double threshold,  vector<PixelCoordinates> &points);
	vector<PixelCoordinates> borderFind(GrayImage *src);	//prepravljena edgeDetectors funkcija boundaryFollower
	vector<PixelCoordinates> followBoundary(GrayImage *origImg, PixelCoordinates moon, PixelCoordinates planet); //prepravljena edgeDetectors funkcija drawBoundary 
};


#endif