#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
#include "paramVector.h"
#include "generateHyp.h"
#include "math.h"


using namespace std;


using namespace std;

bool segmentsCmp( const EdgeSegment &a, const EdgeSegment &b ){
		return (a.getLength() > b.getLength());
}

vector<EdgeSegment> GetLongestSegs(int numOfSeg, vector<EdgeSegment> &segments){
	
	vector<EdgeSegment> bestHyp;
	
	sort( segments.begin(), segments.end(), segmentsCmp );

	if (segments.size()<numOfSeg)//ako ima manje od 10 lin. segmenata
	{
		numOfSeg = segments.size();
	}
	for (int i = 0; i <numOfSeg; i++)  {
	bestHyp.push_back(segments[i]);
	}

	return bestHyp;
}

bool checkCompatibility(EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength)
{
	double angleComp = fabs(M.getAngle_A() - S.getAngle_A());
	double lengthComp = fabs(M.getLength() - S.getLength());

	if (angleComp  > tresholdAngle)
		return false;
	else if (lengthComp > tresholdLength)
		return false;
	else
		return true;
}

bool GenerateErrCovMatrix(EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength)
{
	double k0 = S.getLength() - M.getLength();
	double angle = S.getAngle() - M.getAngle();
	double tx0 = S.getMiddleX() - k0*(M.getMiddleX()*cos(angle) - M.getMiddleY()*sin(angle));
	double ty0 = S.getMiddleY() - k0*(M.getMiddleX()*sin(angle) - M.getMiddleY()*cos(angle));

	bool compatible = checkCompatibility(M, S, tresholdAngle, tresholdLength*k0);

	if (!compatible)
		return false;

	//paramVector v0(ko, angle, tx, ty);
}
