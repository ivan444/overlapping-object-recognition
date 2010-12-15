#include "edgeSegmentator.h"
#include <iostream>
#include <cmath>

EdgeSegmentator::EdgeSegmentator(){}
EdgeSegmentator::~EdgeSegmentator(){}

vector<EdgeSegment> EdgeSegmentator::extractFeatures (GrayImage *src, double threshold, string id)
{
	vector<EdgeSegment> features;
	//GrayImage *dst;
	vector<PixelCoordinates> points = this->borderFind(src);
	vector<PixelCoordinates> points1;
	vector<PixelCoordinates> points2;
	points2 = points1 = points;
	int i1 = 0;
	int i2 = points1.size()/2; 
	PixelCoordinates first = points1[i1];
	PixelCoordinates last = points1[i2];
	points1.erase(points1.begin()+i2+1,points1.end());
	points2.erase(points2.begin(),points2.begin()+i2);
	points2.push_back(points1[0]);
	//EdgeSegment initSegment1(first, last);
	//EdgeSegment initSegment1(last, first);

	
	//divide and conquer edge segmentation
	vector<EdgeSegment> segments1 = this->divideAndConquer(threshold, points1, id);
	vector<EdgeSegment> segments2 = this->divideAndConquer(threshold,  points2, id);
	features = segments1;
	features.insert(features.end(),segments2.begin(),segments2.end());

	//raèuna kut izmeðu trenutnog segmenta i prethodnog
	double alpha, beta;
	alpha = features[features.size()-1].getAngle();
	beta = features[0].getAngle();
	features[0].setAngle_A(alpha - beta);
	for (int i = 1; i<features.size(); i++)
	{
		alpha = features[i-1].getAngle();
		beta = features[i].getAngle();
		features[i].setAngle_A(alpha - beta);
	}
	return features;
}


vector<PixelCoordinates> EdgeSegmentator::borderFind(GrayImage *img)
{
	vector<PixelCoordinates> points;
	//GrayImage *boundImg = new GrayImage(img->getWidth(), img->getHeight());
	PixelCoordinates white, black;
	bool isOver = false;
	for(int y = 0; y < img->getHeight(); y++){
		for(int x = 1; x < img->getWidth(); x++){
			if(img->get(x-1,y).v == 255 && img->get(x,y).v == 0){
				white.x = x-1; white.y = y;
				black.x = x; black.y = y;
				points = followBoundary(img, white, black);
				isOver = true;
				break;
			}
			if(isOver) break;
		}
	}
	return points;
}

vector<PixelCoordinates> EdgeSegmentator::followBoundary(GrayImage *origImg, 
						PixelCoordinates moon, PixelCoordinates planet){
	vector<PixelCoordinates> points;
	int xOffset[8] = {-1,-1,0,1,1,1,0,-1};
	int yOffset[8] = {0,-1,-1,-1,0,1,1,1};
	int offsetIter = 0;
	GrayPixel white;
	white.v = 255;
	//boundImg->set(planet.x, planet.y, white);
	PixelCoordinates start;
	start.x = planet.x;
	start.y = planet.y;
	points.push_back(start);
	for(int k = 0; k < origImg->getWidth()*origImg->getHeight(); k++){
		for(int i = 1; i < 8; i++){
			int orbitX = (planet.x+xOffset[(offsetIter+i)%8])%origImg->getWidth();
			int orbitY = (planet.y+yOffset[(offsetIter+i)%8])%origImg->getHeight();
			if(origImg->get(orbitX, orbitY).v == 0){
				planet.x = orbitX;
				planet.y = orbitY;
				//boundImg->set(planet.x, planet.y, white);
				points.push_back(planet);
				offsetIter = ((int)((offsetIter+i)/2)+3)%4*2;
				break;
			}

		}
		if(start.x == planet.x && start.y == planet.y) break;

	}
	return points;
}



vector<EdgeSegment> EdgeSegmentator::divideAndConquer(double threshold,  vector<PixelCoordinates> &points, string id)
{
	EdgeSegment segment(points[0], points[points.size()-1], id);
	double dx = ((int) segment.getLast().x)- ((int) segment.getFirst().x);
	double dy = ((int) segment.getLast().y)- ((int) segment.getFirst().y);
	double a = dy;
	double b = -dx;
	double c =dx*segment.getFirst().y -dy*segment.getFirst().x;
	int iMax= -1, jMax= -1;

	double maxD = 0;
	PixelCoordinates maxPoint;
	for (int i = 0; i<points.size(); i++)
	{
		double d = points[i].x*a + points[i].y*b + c;
		d = abs (d/sqrt(a*a + b*b));
		if (d > maxD)
		{
			maxD = d;
			maxPoint.x = points[i].x;
			maxPoint.y = points[i].y;
			iMax = i;
		}
	}
	vector<EdgeSegment> segments;
	if (maxD > threshold)
	{
		//EdgeSegment s1(segment.getFirst(),maxPoint);
		//EdgeSegment s2(maxPoint, segment.getLast());
		vector<PixelCoordinates> points1, points2;
		points1.insert(points1.end(), points.begin(), points.begin() + iMax+1);
		points2.insert(points2.end(), points.begin()+iMax, points.end());
		//points2.push_back(points[0]);
		vector<EdgeSegment> segs1 = divideAndConquer(threshold, points1, id);
		vector<EdgeSegment> segs2 = divideAndConquer(threshold, points2, id);
		segments = segs1;
		segments.insert(segments.end(),segs2.begin(),segs2.end());
		//if (segs2.back().getAngle() != segments.back().getAngle())
			//segments.push_back(segs2.back());
	}
	else
	{
		segments.push_back(segment);
	}
	return segments;
}
