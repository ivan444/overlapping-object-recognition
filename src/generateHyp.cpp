#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
//#include "paramVector.h"
#include "generateHyp.h"
#include "math.h"
#include "image.h"
#define M_PI       3.14159265358979323846


#define sk 0.1
#define sa 0.1
#define sx 0.1
#define sy 0.1

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
	double new_x = tx + x*kCos - y*kSin;
	double new_y = ty + x*kSin + y*kCos;
	PixelCoordinates tFirst;
	tFirst.x = (unsigned int) round (new_x);
	tFirst.y = (unsigned int) round (new_y);

	x = (double)last.x;
	y = (double)last.y;
	new_x = tx + x*kCos - y*kSin;
	new_y = ty + x*kSin + y*kCos;
	PixelCoordinates tLast;
	tLast.x = (unsigned int) round (new_x);
	tLast.y = (unsigned int) round (new_y);

	EdgeSegment result(tFirst,tLast, "transform" + orig.getImagrID());


	return result;
}

bool segmentsCmp( const EdgeSegment &a, const EdgeSegment &b ){
		return (a.getLength() > b.getLength());
}

vector<EdgeSegment> GetLongestSegs(int numOfSeg, vector<EdgeSegment> segments){
	
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
	double angleM = atan(tan(M.getAngle_A()));
	if (angleM < 0) angleM += M_PI;
	double angleS = atan(tan(S.getAngle_A()));
	if (angleS < 0) angleS += M_PI;

	A = fabs(angleM - angleS);
	r = fabs(S.getLength()/ M.getLength());

	if(! (A < treshAngle || fabs(A - M_PI) < treshAngle)) return false;
	else if (r - k0 > k0*treshLength) return false; // Zasad nepotrebno...
	else if (k0 > 5 || k0 < 0.2) return false; // @Debug, uklanjanje loših hipoteza (svi objekti su nam približno jednaki)
	else return true;
}

bool GenerateErrCovMatrix(Hypothesis &hypothesis, EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength)
{
	double A = 0;
	double r = 0;
	// @Debug
	if (S.getMiddleX() == 641 && M.getMiddleX() == 653)
		int nesto = 0;
	if (M.getAngle_A() > 1.02 && M.getAngle_A() < 1.03)
		int nesto = 12843;
	double k0 = fabs(S.getLength()/M.getLength()); // zasad nekorisno
	double angle = S.getAngle() - M.getAngle();
	double tx0 = S.getMiddleX() - k0*(M.getMiddleX()*cos(angle) - M.getMiddleY()*sin(angle));
	double ty0 = S.getMiddleY() - k0*(M.getMiddleX()*sin(angle) + M.getMiddleY()*cos(angle));

	bool compatible = checkCompatibility(M, S, tresholdAngle, tresholdLength, k0, A, r);

	if (!compatible) return false;

	paramVector v0(k0, angle, tx0, ty0);

	hypothesis.setV(v0);
	hypothesis.setMseg(M);
	hypothesis.setSseg(S);
	hypothesis.setAngleComp(A);
	hypothesis.setLengthComp(r);

	ublas::matrix<double> matrixS = ublas::identity_matrix<double> (4);
	matrixS(0,0) = k0*k0*sin(angle)*sin(angle)*sa*sa + cos(angle)*cos(angle)*sk*sk;
	matrixS(1,1) = k0*k0*cos(angle)*cos(angle)*sa*sa + sin(angle)*sin(angle)*sk*sk;
	matrixS(0,1) = matrixS(1,0) = sin(angle)*cos(angle)*(sk*sk - k0*k0*sa*sa);
	matrixS(2,2) = sx*sx;
	matrixS(3,3) = sy*sy;
	hypothesis.setMatrixS(matrixS);

	return true;
}


bool hypothesisCmp(  Hypothesis &a,  Hypothesis &b ){
	// TODO: Eksperimentirati
	return !(a.getAngleComp()+a.getLengthComp() > b.getAngleComp()+b.getLengthComp());
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
	//bestHyps = hyps;

	return bestHyps;
}