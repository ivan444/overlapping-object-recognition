#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "edgeSegmentator.h"


using namespace std;

bool segmentsCmp( const EdgeSegment &a, const EdgeSegment &b );
vector<EdgeSegment> GetBestHyp(int numOfHyp, vector<EdgeSegment> &segments);

