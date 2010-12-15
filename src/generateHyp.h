#include <iostream>
#include <string>
#include "imageIO.h"
#include "math.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
//#include "paramVector.h"


using namespace std;

vector<EdgeSegment> GetLongestSegs(int numOfHyp, vector<EdgeSegment> &segments);

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

//class Hypothesis
//{
//private:
//	paramVector v;
//	double angleComp;
//	double lengthComp;
//
//public:
//	Hypothesis(paramVector vec, double ang, double length){
//		v=vec;
//		angleComp = ang;
//		lengthComp = length;
//	}
//
//	~Hypothesis(){}
//	paramVector getV(){return v;}
//	double getAngleComp(){return angleComp;}
//	double getLengthComp(){return angleComp;}
//	
//};