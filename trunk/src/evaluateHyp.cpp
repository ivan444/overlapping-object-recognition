#include "edgeSegmentator.h"
#include "generateHyp.h"
#include <cmath>
#include <boost/numeric/ublas/matrix.hpp>
#include "matrix/math_tnt_la.hpp"
#include <vector>

#define aMax 0.35
#define DMax 12.0
#define lMax 0.7
#define p 0.6
#define q 0.3
#define r 0.1

using namespace boost::numeric::ublas;

using namespace math::tnt;

using namespace std;


double updateBasic(Hypothesis &H, double lMean)
{
	EdgeSegment mi = H.getMseg();
	EdgeSegment si = H.getSseg();
	//stvori vi-1
	matrix<double> vi1(4,1);
	vi1(0,0) = H.getV().getKCos();
	vi1(1,0) = H.getV().getKSin();
	vi1(2,0) = H.getV().getTx();
	vi1(3,0) = H.getV().getTy();
	matrix<double> errorSi1 = H.getMatrixS();
	matrix<double> Ki;
	// stvori Yi
	matrix<double> Yi(2,1);
	Yi(0,0) = si.getMiddleX();
	Yi(1,0) = si.getMiddleY();
	//stvori Ci
	matrix<double> Ci(2,4);
	Ci(0,0) = mi.getMiddleX(); Ci(0,1) = -mi.getMiddleY(); Ci(0,2) = 1; Ci(0,3) = 0;
	Ci(1,0) = mi.getMiddleY(); Ci(1,1) = mi.getMiddleX(); Ci(1,2) = 0; Ci(1,3) = 1;
	double wi = (DMax/lMean)/mi.getLength();
	//stvori Ci'
	matrix<double> tCi = trans(Ci);
	//stvori Wi
	matrix<double> Wi = identity_matrix<double> (2);
	Wi(0,0)= (wi*wi);
	Wi(1,1)= (wi*wi);
	//stvori I dimenzije 4x4
	matrix<double> I = identity_matrix<double> (4);
	//izraèunaj Ki
	matrix<double> temp = prod(Ci, errorSi1); //Ci*Si1
	temp = prod(temp, tCi);//Ci*Si1*Ci'
	temp += Wi; // (Ci*Si1*Ci' + Wi)
	matrix<double> temp2;
	math::tnt::invert(temp, temp2); // inv(Ci*Si-1*Ci' + Wi)
	Ki = ublas::prod(errorSi1, tCi);
	Ki= ublas::prod(Ki, temp2);
	temp.clear();
	temp2.clear();

	//Izraèunaj Si pogreške
	temp = prod(Ki,Ci);
	matrix<double> errorSi = I;
	errorSi -= temp;
	errorSi = prod(errorSi, errorSi1);
	temp.clear();
	//izraèunaj vi;
	temp = prod(Ci,vi1); 
	temp2 = Yi;
	temp2 -= temp;
	matrix<double> vi = prod(Ki, temp2);
	vi += vi1;
	temp.clear();
	temp2.clear();
	double angle = atan(vi(1,0)/vi(0,0));
	if (angle < 0) angle += M_PI;

	double k = vi(0,0)/acos(angle);
	paramVector _vi(k, angle, vi(2,0), vi(3,0));
	H.setV(_vi);
	

	//izraèunaj R
	/*temp = prod(Ci,vi);
	temp2 = Yi;
	temp2 -= temp;
	ublas::matrix<double> iWi;
	invert(Wi, iWi);
	temp.clear();
	temp = prod(trans(temp2),iWi);
	ublas::matrix<double>mRi = prod(mRi, temp2);
	double Ri = mRi(0,0);
	//Ri += Ri+1;
	temp.clear();
	temp2.clear();
	return Ri;*/
	return 0;
}


int findSegment(EdgeSegment s, std::vector<EdgeSegment> ss)
{
	double c = 0.0001;
	for (int i = 0; i < ss.size(); i++)
	{
		if (( fabs(s.getAngle()-ss[i].getAngle()) < c) &&
			( fabs(s.getMiddleX()-ss[i].getMiddleX()) < c) &&
			( fabs(s.getMiddleY()-ss[i].getMiddleY()) < c) &&
			( fabs(s.getLength()-ss[i].getLength()) < c))
			return i;
	}
	return -1;
}

double avgLength( std::vector<EdgeSegment> &ss)
{
	double avg = 0;
	for (int i = 0; i < ss.size(); i++)
	{
		avg+= ss[i].getLength();
	}
	avg = avg/((double) (ss.size()));
	return avg;
}

double sumLength( std::vector<EdgeSegment> &ss)
{
	double sum = 0;
	for (int i = 0; i < ss.size(); i++)
	{
		sum+= ss[i].getLength();
	}
	return sum;
}

// funkcija match vraæa 
/*int match(paramVector v,
		  std::vector<EdgeSegment> &scene, std::vector<EdgeSegment> &model,
		  int startSegmentModel)*/
double match(Hypothesis &initH, std::vector<EdgeSegment> &scene,
			 std::vector<EdgeSegment> &model,
			 std::vector<int> &matchedScene,
			 std::vector<int> &matchedModelInd)
{
	matchedScene.clear();
	paramVector v = initH.getV();
	int startSegmentModel = findSegment(initH.getMseg(), model);
	int startSegmentScene = findSegment(initH.getSseg(), scene);
	if(startSegmentModel == -1 || startSegmentScene == -1)
		return -1;

	Hypothesis H;
	H.setMatrixS(initH.getMatrixS());
	H.setV(initH.getV());
	H.setMseg(initH.getMseg());
	H.setSseg(initH.getSseg());
	matchedScene.push_back(startSegmentScene);
	matchedModelInd.push_back(startSegmentModel);

	double lMean = avgLength (model);
	std::vector<EdgeSegment> matchedModel;
	matchedModel.push_back(model[startSegmentModel]);
	double sumModel = sumLength(model);
	double sumMatchedModel = model[startSegmentModel].getLength();
	double Qi = sumMatchedModel/sumModel;

	int direction = 1;
	int step = 1;
	int i;
	int iLast = 0;
	int iOffset = 0;
	while(1)
	{
		if (direction == 1)
		{
			iOffset+=step;
			i = startSegmentModel + iOffset + model.size();
			i= i% model.size();
			direction = -1;
		}
		else if (direction == -1)
		{
			i = startSegmentModel - iOffset + model.size();
			i= i% model.size();
			direction = 1;
		}
		if (i == iLast)
			break;
		iLast = i;

		EdgeSegment Mi = model[i];
		EdgeSegment tMi = v.transform(Mi);
		double dMin = 1;
		int jMin = -1;
		for (int j = 0; j<scene.size(); j++)
		{
			EdgeSegment Sj = scene[j];
			double dij = 0;
			double aij = tMi.getAngle() - Sj.getAngle();
			double x1 = tMi.getMiddleX();
			double x2 = Sj.getMiddleX();
			double y1 = tMi.getMiddleY();
			double y2 = Sj.getMiddleY();
			double Dij = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			double lij = fabs((tMi.getLength()-Sj.getLength())/Sj.getLength());
			
			if(aij >= aMax || Dij >= DMax || lij >= lMax)
			{
				dij = 1.0;
			}
			else
			{
				dij = p* (aij/aMax) + q*(Dij/DMax) + r*(lij/lMax);
			}

			if (dij < dMin)
			{
				dMin = dij;
				jMin = j;
				
			}

		}
		if (dMin <1)
		{
			H.setMseg(Mi);
			H.setSseg(scene[jMin]);
			matchedScene.push_back(jMin);
			matchedModelInd.push_back(i);
			sumMatchedModel += Mi.getLength();
			double Ri = updateBasic(H, lMean);
			Qi = sumMatchedModel/sumModel;
		}
		
	}
	sort(matchedScene.begin(), matchedScene.end());
	return Qi;
}





