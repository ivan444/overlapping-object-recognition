#include <iostream>
#include <string>
#include "imageIO.h"
#include "math.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"


using namespace std;

class paramVector
{
private:
	double k0;
	double angle;
	double tx0;
	double ty0;
	double k0sin;
	double k0cos;

public:
	paramVector(){}
	paramVector(double k, double ang, double tx, double ty){
		k0 = k;
		angle = ang;
		tx0 = tx;
		ty0 = ty;
		k0sin = k0 * sin(angle);
		k0cos = k0 * cos(angle);
	}

	~paramVector(){}
	double getK0sin(){return k0sin;}
	double getK0cos(){return k0cos;}
	
};

class Hypothesis
{
private:
	paramVector v;
	EdgeSegment Mseg;
	EdgeSegment Sseg;
	double angleComp;
	double lengthComp;

public:
	Hypothesis(){}
	Hypothesis(paramVector vec, double ang, double length, EdgeSegment M, EdgeSegment S){
		v=vec;
		angleComp = ang;
		lengthComp = length;
		Mseg = M;
		Sseg = S;
	}

	~Hypothesis(){}
	paramVector getV(){return v;}
	double getAngleComp(){return angleComp;}
	double getLengthComp(){return angleComp;}
	EdgeSegment getMseg(){return Mseg;}
	EdgeSegment getSseg(){return Sseg;}

	void setV(paramVector value){v=value;}
	void setMseg(EdgeSegment value){Mseg=value;}
	void setSseg(EdgeSegment value){Sseg=value;}
	void setAngleComp(double value){angleComp=value;}
	void setLengthComp(double value){lengthComp=value;}
	
};

vector<EdgeSegment> GetLongestSegs(int numOfHyp, vector<EdgeSegment> &segments);
bool GenerateErrCovMatrix(Hypothesis &hypothesis, EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength);
