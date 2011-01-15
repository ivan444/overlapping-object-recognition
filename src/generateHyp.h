#ifndef _GEN_HYP_H
#define _GEN_HYP_H

#include <iostream>
#include <string>
#include "imageIO.h"
#include "math.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
#include <boost/numeric/ublas/matrix.hpp>


using namespace std;

using namespace boost::numeric;

int round(double x);

class paramVector
{
private:
	double k;
	double angle;
	double tx;
	double ty;
	double kSin;
	double kCos;

public:
	paramVector(){}
	paramVector(double _k, double _ang, double _tx, double _ty){
		k = _k;
		angle = _ang;
		tx = _tx;
		ty = _ty;
		kSin = k * sin(angle);
		kCos = k * cos(angle);
	}

	~paramVector(){}
	double getKSin(){return kSin;}
	double getKCos(){return kCos;}
	double getTx(){return tx;}
	double getTy(){return ty;}

	/*double setKSin(){return kSin;}
	double setKCos(){return kCos;}
	double setTx(){return tx;}
	double setTy(){return ty;}*/
	EdgeSegment transform(EdgeSegment &orig);
	
};

class Hypothesis
{
private:
	paramVector v;
	EdgeSegment Mseg;
	EdgeSegment Sseg;
	double angleComp;
	double lengthComp;
	ublas::matrix<double> matrixS;

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
	ublas::matrix<double> &getMatrixS() {return matrixS;}

	void setV(paramVector value){v=value;}
	void setMseg(EdgeSegment value){Mseg=value;}
	void setSseg(EdgeSegment value){Sseg=value;}
	void setAngleComp(double value){angleComp=value;}
	void setLengthComp(double value){lengthComp=value;}
	void setMatrixS(ublas::matrix<double> value){matrixS=value;}
	
};

vector<EdgeSegment> GetLongestSegs(int numOfHyp, vector<EdgeSegment> &segments);
bool GenerateErrCovMatrix(Hypothesis &hypothesis, EdgeSegment M, EdgeSegment S, double tresholdAngle, double tresholdLength);
vector<Hypothesis> getBestHyp(int numOfHyp, vector<Hypothesis>  &hyps);

int match(Hypothesis &initH, std::vector<EdgeSegment> &scene, std::vector<EdgeSegment> &model);

#endif