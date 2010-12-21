#include "edgeSegmentator.h"
#include "generateHyp.h"
#include <cmath>

using namespace std;

double match(paramVector v,
			 vector<EdgeSegment> &scene, vector<EdgeSegment> &model,
			 int startSegmentScene, int startSegmentModel)
{
	int direction = 1;
	int step = 1;
	int i;
	int iOffset = 0;
	while(1)
	{
		if (direction == 1)
		{
			iOffset+=step;
			i = startSegmentModel + iOffset;
			i= i% model.size();
			direction = -1;
		}
		else if (direction == -1)
		{
			i = startSegmentModel - iOffset;
			i= i% model.size();
			direction = 1;
		}

		EdgeSegment Mi = model[i];
		EdgeSegment tMi = v.transform(Mi);
		vector<double> ai;
		vector<double> Di;
		vector<double> li;
		vector<double> ai;
		vector<double> di;
		for (int j = 0; j<scene.size(); j++)
		{
			EdgeSegment Sj = scene[j];
			double aij = tMi.getAngle() - Sj.getAngle();
			double x1 = tMi.getMiddleX();
			double x2 = Sj.getMiddleX();
			double y1 = tMi.getMiddleY();
			double y2 = Sj.getMiddleY();
			double Dij = sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
			//double li = tMi.getFirst

			ai.push_back(aij);

		}
	}
}

