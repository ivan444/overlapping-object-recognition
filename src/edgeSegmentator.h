#ifndef _EDGE_SEGMENT_H
#define _EDGE_SEGMENT_H

#include <vector>
#include <string>
#include "image.h"
#include <cmath>
using namespace std;


class EdgeSegment
{
private:
	double angle;
	double angle_A;
	double angle_atan;
	double length;
	double middleX;
	double middleY;
	PixelCoordinates first;
	PixelCoordinates last;
	string imagrID;
public:
	EdgeSegment(){};
	EdgeSegment(PixelCoordinates _first, PixelCoordinates _last, string id){
		first = _first;
		last = _last;
		middleX = (first.x+last.x)*0.5;
		middleY = (first.y+last.y)*0.5;
		double dx = ((int)(last.x)) - ((int)first.x);
		double dy = ((int)last.y) - ((int)first.y);
		length = sqrt(dx*dx + dy*dy);
		angle = atan2(dy,dx);
		angle_atan = atan(dy/dx);
		angle_A=0;
		imagrID = id;
	}

	~EdgeSegment(){}
	/** Kut u odnosu na x-os. */
	double getAngle(){return angle;}
	/** Kut u odnosu na prethodni segment. */
	double getAngle_A(){return angle_A;}
	/** Kut u odnosu na x-os ali je dobiven funkcijom atan. */
	double getAngle_atan(){return angle_atan;}
	void setAngle_A(double value){angle_A=value;}
	double getLength()const{return length;}
	double getMiddleX (){return middleX;}
	double getMiddleY() {return middleY;}
	string getImagrID(){ return imagrID;}
	PixelCoordinates getFirst() {return first;}
	PixelCoordinates getLast() {return last;}

	void setFirst(unsigned int x, unsigned int y) {
		first.x = x;
		first.y = y;
	}

	void setLast(unsigned int x, unsigned int y) {
		last.x = x;
		last.y = y;
	}

	void setFirst(PixelCoordinates f) {
		first = f;
	}

	void setLast(PixelCoordinates l) {
		last = l;
	}

	/*EdgeSegment& operator= (EdgeSegment &seg)
	{
		angle = seg.angle;
		angle_A = seg.angle_A;
		length = seg.length;
		middleX = seg.middleX;
		middleY = seg.middleY;
		first = seg.first;
		last = seg.last;
		imagrID = "transform" + seg.imagrID;
		return *this;
	}*/
};

class EdgeSegmentator
{
	
public:
	EdgeSegmentator();
	~EdgeSegmentator();
	vector<EdgeSegment> extractFeatures (GrayImage *src, double threshold, string id, double angleThreshold);
private:
	vector<EdgeSegment> divideAndConquer( double threshold,  vector<PixelCoordinates> &points, string id);
	vector<PixelCoordinates> borderFind(GrayImage *src);	//prepravljena edgeDetectors funkcija boundaryFollower
	vector<PixelCoordinates> followBoundary(GrayImage *origImg, PixelCoordinates moon, PixelCoordinates planet); //prepravljena edgeDetectors funkcija drawBoundary 
};


#endif