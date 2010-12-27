#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
//#include "paramVector.h"
#include "generateHyp.h"
#include "math.h"
#include "image.h"


using namespace std;

int round(double x)
{
	int intX = (int) x;
	x -= (double) intX;
	if (x < 0.5)
		return intX;
	else
		return intX+1;
};

EdgeSegment paramVector::transform(EdgeSegment &orig)
{
	PixelCoordinates first = orig.getFirst();
	PixelCoordinates last = orig.getLast();
	double x = (double)first.x;
	double y = (double)first.y;
	x = tx + x*kCos - y*kSin;
	y = ty + x*kSin + y*kCos;
	PixelCoordinates tFirst;
	tFirst.x = (unsigned int) round (x);
	tFirst.y = (unsigned int) round (y);

	x = (double)last.x;
	y = (double)last.y;
	x = tx + x*kCos - y*kSin;
	y = ty + x*kSin + y*kCos;
	PixelCoordinates tLast;
	tLast.x = (unsigned int) round (x);
	tLast.y = (unsigned int) round (y);

	EdgeSegment result(tFirst,tLast, "transform" + orig.getImagrID());


	return result;
}

bool segmentsCmp( const EdgeSegment &a, const EdgeSegment &b ){
		return (a.getLength() > b.getLength());
}

vector<EdgeSegment> GetLongestSegs(int numOfSeg, vector<EdgeSegment> &segments){
	
	vector<EdgeSegment> longestSegs;
	
	sort( segments.begin(), segments.end(), segmentsCmp );

	if (segments.size()< numOfSeg)//ako ima manje od 10 lin. segmenata
	{
		numOfSeg = segments.size();
	}
	for (int i = 0; i <numOfSeg; i++)  {
	longestSegs.push_back(segments[i]);
	}

	return longestSegs;
}

bool checkCompatibility(EdgeSegment M, EdgeSegment S, double treshAngle, double treshLength, double k0, double &A, double &r)
{
	A = fabs(M.getAngle_A() - S.getAngle_A());
	r = fabs(M.getLength() - S.getLength());

	if (A  > treshAngle)
		return false;
	else if (r - k0 > k0*treshLength)
		return false;
	else
		return true;
}

bool GenerateErrCovMatrix(Hypothesis &hypothesis, EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength)
{
	double A = 0;
	double r = 0;
	double k0 = fabs(S.getLength()/M.getLength());
	double angle = S.getAngle() - M.getAngle();
	double tx0 = S.getMiddleX() - k0*(M.getMiddleX()*cos(angle) - M.getMiddleY()*sin(angle));
	double ty0 = S.getMiddleY() - k0*(M.getMiddleX()*sin(angle) + M.getMiddleY()*cos(angle));

	bool compatible = checkCompatibility(M, S, tresholdAngle, tresholdLength, k0, A, r);

	if (!compatible)
		return false;

	paramVector v0(k0, angle, tx0, ty0);

	hypothesis.setV(v0);
	hypothesis.setMseg(M);
	hypothesis.setSseg(S);
	hypothesis.setAngleComp(A);
	hypothesis.setLengthComp(r);

	return true;
}


bool hypothesisCmp(  Hypothesis &a,  Hypothesis &b ){
		return (a.getAngleComp()+a.getLengthComp() > a.getAngleComp()+a.getLengthComp());
}

vector<Hypothesis> getBestHyp(int numOfHyp, vector<Hypothesis>  &hyps){
	
	vector<Hypothesis> bestHyps;
	
	sort( hyps.begin(), hyps.end(), hypothesisCmp );

	if (hyps.size()< numOfHyp)
	{
		numOfHyp = hyps.size();
	}
	for (int i = 0; i <numOfHyp; i++)  {
	bestHyps.push_back(hyps[i]);
	}

	return bestHyps;
}