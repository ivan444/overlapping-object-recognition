#include <iostream>
#include <string>
#include <cstdlib>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "binaryFilter.h"
#include "edgeDetectors.h"
#include "edgeSegmentator.h"
#include "annotate.h"
#include "generateHyp.h"
#include <cmath>
#include "getopts.h"

#define M_PI       3.14159265358979323846

using namespace std;

bool hypothesisCmp2(  Hypothesis &a,  Hypothesis &b ){
		//return (a.getAngleComp()+a.getLengthComp() > b.getAngleComp()+b.getLengthComp());
	return (a.getQ() > b.getQ());
}

/*
 * Crtanje segmenata objekta i ispis njegovih podataka.
 */
void writePolys(vector<vector<EdgeSegment>> segments) {
	for (int i = 0; i < segments.size(); i++) {
		cout << "Model: " << segments[i][0].getImagrID() << ", broj segmenata: " << segments[i].size() << endl;
		
		string imgId = segments[i][0].getImagrID();
		string outDir = "../anotirani/";
		string inDir = "../baza/";

		ImageIO *io = new JpegIO();
		BinaryFilter* bf = new BinaryFilter(150);

		ColorImage *orgModelImg = io->read((char*)(inDir+imgId).c_str());
		GrayImage *gModelImage = bf->applyFilterC2G(orgModelImg);

		cout << "Anotiranje!"<< endl;
		ColorImage *mAnotImg = annotate(gModelImage, segments[i]);

		char num[10];
		itoa(i, num, 10);

		cout << "Spremanje!" << endl;
		string outFile = outDir + imgId;
		outFile.append(num);
		outFile.append(".jpg");
		io->write(mAnotImg, (char*)outFile.c_str());		
	}
}

void loadModels(vector<string> &modelShapes, string modelsPath,
	ImageIO *io, BinaryFilter* bf,
	int eFpar, double angleTreshold, int numOfSeg_model,
	vector<vector<EdgeSegment>> &modelSegments,
	vector<vector<EdgeSegment>> &modelAllSegments) {
		
	for (int i = 0; i < modelShapes.size(); i++) {
		string imageID = modelShapes[i];
		string fileRead = modelsPath + imageID;
		cout << "reading " << fileRead <<endl;
		ColorImage *img = io->read((char*) fileRead.c_str());
		if (img == NULL) continue;
		GrayImage *gImg = bf->applyFilterC2G(img);
		delete img;
		EdgeSegmentator f;
		vector<EdgeSegment> segments = f.extractFeatures(gImg, eFpar, imageID, angleTreshold);
		vector<EdgeSegment> longestSegs = GetLongestSegs(numOfSeg_model, segments); 

		modelSegments.push_back(longestSegs);
		modelAllSegments.push_back(segments);
	}
}

void generateHyps(int numOfSeg_scene, vector<EdgeSegment> segments,
			vector<vector<EdgeSegment>> modelSegments,
			double tresholdAngle, double tresholdLength,
			vector<Hypothesis> &CompHyps) {

	vector<EdgeSegment> longestSegs  = GetLongestSegs(numOfSeg_scene, segments); 
	for ( int i = 0; i < longestSegs.size(); i++) {
		for ( int j = 0; j < modelSegments.size(); j++) {
			for ( int k = 0; k < modelSegments[j].size(); k++) {
				Hypothesis hypothesis;
				bool compatible = GenerateErrCovMatrix(hypothesis,
									modelSegments[j][k], longestSegs[i],
									tresholdAngle, tresholdLength);

				if (compatible) {
					CompHyps.push_back(hypothesis);
				}
			}
		}
	}
}


void evaluateHyps(vector<Hypothesis> &BestHyps, vector<EdgeSegment> &segments,
				  vector<vector<EdgeSegment>> &modelAllSegments,
				  double &Qmax, double &iBest, string &idBest) {

	for (int i = 0; i <BestHyps.size(); i++)
	{
		std::vector<int> matchedScene;
		std::vector<int> matchedModels;
		int j = -1;
		for(int k = 0; k < modelAllSegments.size(); k++)
		{
			if (BestHyps[i].getMseg().getImagrID() == modelAllSegments[k][0].getImagrID())
			{
				j = k;
				break;
			}
		}
		
		Hypothesis newH;
		//cout << "\nEvaluating hypothesis: " << i <<" " << modelAllSegments[j][0].getImagrID();
		if (i == 2 || i == 4 || i == 7)
			int nesto = 0;
		double Qi = match(BestHyps[i],newH, segments, modelAllSegments[j], matchedScene, matchedModels);
		if (Qi > Qmax)
		{
			iBest = i;
			Qmax = Qi;
			idBest = modelAllSegments[j][0].getImagrID();
		}
		BestHyps[i].setQ(Qi);
		//cout << "\nQuality of hypothesis: " << Qi <<"\n";
	}
}

bool parseOptions(int argc, char *argv[], double *minPolyAngle, int *minPolyDotDist,
		double *minHypCompAngle, double *minHypCompLength, int *sceneSegsNum,
		int *modelSegsNum, int *numOfHyp, string *modelsDir, string *scenePath,
		vector<string> &modelShapes)
{
    char* minPolyAngle_ = NULL;
	char* minPolyDotDist_ = NULL;
	char* minHypCompAngle_ = NULL;
	char* minHypCompLength_ = NULL;
	char* sceneSegsNum_ = NULL;
	char* modelSegsNum_ = NULL;
	char* numOfHyp_ = NULL;
	char* modelsDir_ = NULL;
	char* scenePath_ = NULL;
	char* modelShapes_ = NULL;
	char* debug = NULL; // not used
	bool help;
	
	bool hasRequired = true;

    struct options opts[] = 
    {
       { &minPolyAngle_, "polyangle", "Threshold angle for connecting polygons.", "a", 1 },
	   { &minPolyDotDist_, "polydotdist", "Threshold for connecting dots into polygons.", "i", 1 },
	   { &minHypCompAngle_, "hypcompangle", "Threshold angle for compatible hypotesis.", "c", 1 },
	   { &minHypCompLength_, "hypcomplength", "Threshold length for compatible hypotesis.", "l", 1 },
	   { &sceneSegsNum_, "scenesegsnum", "Number of segments in scene.", "g", 1 },
	   { &modelSegsNum_, "modelsegsnum", "Number of segments in model.", "j", 1 },
	   { &numOfHyp_, "hypsnum", "Number of hypotesis.", "f", 1 },
	   { &modelsDir_, "modelsdir", "Path to directory with models.", "b", 1 },
	   { &scenePath_, "scenepath", "Path to scene image.", "s", 1 },
	   { &modelShapes_, "models", "List of models (filenames).", "m", 1 },
	   { &debug, "debug", "Debug mode.", "d", 0 } // Not used

    };

    GetOpts _optprser = GetOpts(opts, help, 11, argc, argv);
	if (help) return false;
    
    if(debug != NULL) {
        printf("Value for debug is: %s\n", debug);
    }

    return hasRequired;
}

void printResults (vector<Hypothesis> &hyps)
{
	vector<string> modelNames;
	vector<double> modelQualities;
	modelNames.push_back(hyps[0].getMseg().getImagrID());
	modelQualities.push_back(hyps[0].getQ());
	for (int i = 1; i < hyps.size(); i++)
	{
		string model = hyps[i].getMseg().getImagrID();
		vector<string>::iterator it;
		it = find (modelNames.begin(), modelNames.end(), model);
		if (it == modelNames.end())
		{
			modelNames.push_back(model);
			modelQualities.push_back(hyps[i].getQ());
		}
	}

	cout << endl;
	for (int i = 0; i < modelNames.size(); i++)
	{
		cout << i+1 << ".)" << modelNames[i] << " " << modelQualities[i] << endl; 
	}
	return;
}


//kasnije: varirati broj segmenata u sceni koje uzimamo u obzir (model 10)
int main(int argc, char *argv[]) {
	
	cout << "Hello\n";
	string imageID;
	double angleTreshold = 15*(M_PI/180);
	int eFpar = 10;  // dozvoljena udaljenost tocke od segmenta prilikom poligonizacije/divide and conquer
	int numOfSeg_scene = 20;
	int numOfSeg_model = 20;
	int numOfHyp = 10000;				//broj hipoteza koje dalje evaluiramo
	int numOfShapes = 12;			//broj oblika u bazi
	double tresholdAngle = 30*(M_PI/180);		//mjera sliènosti kuteva kod provjere kompatibilnosti segmenata
	double tresholdLength = 0.3;	//mjera sliènosti duljina kod provjere kompatibilnosti segmenata
	vector<vector<EdgeSegment>> modelSegments;	//najduži segmenti svih modela
	vector<vector<EdgeSegment>> modelAllSegments;	//svi segmenti svih modela
	vector<string> modelShapes;
	string modelsDir = "../baza/";
	string scenePath;

	bool gotAll = parseOptions(argc, argv, &angleTreshold, &eFpar,
		&tresholdAngle, &tresholdLength, &numOfSeg_scene,
		&numOfSeg_model, &numOfHyp, &modelsDir, &scenePath,
		modelShapes);

	if (!gotAll) {
		return -1;
	}

	ImageIO *io = new JpegIO();
	ColorImage *img = 0;
	
	vector<EdgeSegment> segments;

	BinaryFilter* bf = new BinaryFilter(150);
	EdgeDetector* ed = new EdgeDetector();
	EdgeSegmentator f;

	
	// TODO: Predati preko argumenata!
	modelShapes.push_back("0101.jpg");
	modelShapes.push_back("0201.jpg");
	modelShapes.push_back("0301.jpg");
	modelShapes.push_back("0401.jpg");
	modelShapes.push_back("0501.jpg");
	modelShapes.push_back("0601.jpg");
	modelShapes.push_back("0701.jpg");
	modelShapes.push_back("0801.jpg");
	modelShapes.push_back("0901.jpg");
	modelShapes.push_back("1001.jpg");
	modelShapes.push_back("1101.jpg");
	modelShapes.push_back("1201.jpg");

	loadModels(modelShapes, modelsDir, io, bf, eFpar,
		angleTreshold, numOfSeg_model,
		modelSegments, modelAllSegments);

	// @Debug
	// writePolys(modelAllSegments);

	// TODO: Predati preko argumenata!
	imageID = "0102.jpg";
	string fileNameS = "../baza/"+imageID;
	char *fileName = (char*)fileNameS.c_str();
	img = io->read(fileName);
	
	GrayImage* fImage = bf->applyFilterC2G(img);
	segments = f.extractFeatures(fImage, eFpar, imageID, angleTreshold);
	
	vector<Hypothesis> CompHyps;  //kompatibilne hipoteze
	generateHyps(numOfSeg_scene, segments, modelSegments,
					tresholdAngle, tresholdLength, CompHyps);

	//@Debug
	/*vector<vector<EdgeSegment>> sceneAll;
	sceneAll.push_back(sceneSegments);
	writePolys(sceneAll);*/

	printf("Broj hipoteza: %d\n", CompHyps.size());
	vector<Hypothesis> BestHyps = getBestHyp(numOfHyp, CompHyps); 
	printf("Best hipot: %d\n", BestHyps.size());
	/*for (int i = 0; i < BestHyps.size(); i++) {
		cout << BestHyps[i].getMseg().getImagrID() << endl;
	}*/

	//evaluacija hipoteza
	double Qmax = 0.0;
	double iBest = -1;
	string idBest = "";
	evaluateHyps(BestHyps, segments, modelAllSegments, Qmax, iBest, idBest);

	cout << "\nBest hypothesis is: " << iBest << ", " <<idBest;
	cout << "\nBest quality is: " << Qmax << endl;
	sort( BestHyps.begin(), BestHyps.end(), hypothesisCmp2 );
	/*int bestWriteSize = 20;
	if (bestWriteSize > BestHyps.size()){
		bestWriteSize = BestHyps.size();
	}
	for (int i = 0; i < bestWriteSize; i++)
	{
		cout << i << ".) "<< BestHyps[i].getV().getTx() << "  "  << BestHyps[i].getV().getTy() <<"  "  << BestHyps[i].getV().getAngle() <<"  "  << BestHyps[i].getV().getK() << endl;
		cout << BestHyps[i].getQ() << " " << BestHyps[i].getMseg().getImagrID()<< endl;
	}
	iBest = 0;
	cout << "\nBest of the best\n";
	cout << iBest << ".) "<< BestHyps[iBest].getV().getTx() << "  "  << BestHyps[iBest].getV().getTy() <<"  "  << BestHyps[iBest].getV().getAngle() <<"  "  << BestHyps[iBest].getV().getK() << endl;
	cout << BestHyps[iBest].getQ() << " "<< BestHyps[iBest].getMseg().getImagrID() << endl;*/
	
	printResults(BestHyps);
	int t; cin >> t;
	return 1;
}
