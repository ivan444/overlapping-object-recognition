#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"
#include "generateHyp.h"


using namespace std;


using namespace std;

bool segmentsCmp( const EdgeSegment &a, const EdgeSegment &b ){
		return (a.getLength() > b.getLength());
}

vector<EdgeSegment> GetBestHyp(int numOfHyp, vector<EdgeSegment> &segments){
	
	vector<EdgeSegment> bestHyp;
	
	sort( segments.begin(), segments.end(), segmentsCmp );

	for (int i = 0; i <numOfHyp; i++)  {
	bestHyp.push_back(segments[i]);
	}

	return bestHyp;

}