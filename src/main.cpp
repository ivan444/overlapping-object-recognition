#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "binaryFilter.h"
#include "edgeDetectors.h"
#include "edgeSegmentator.h"
#include "annotate.h"
#include "generateHyp.h"

using namespace std;


//kasnije: varirati broj segmenata u sceni koje uzimamo u obzir (model 10)
int main() {
	
	cout << "Hello\n";
	cout << (-1%3) << endl;
	const double PI = 3.141592;
	string imageID;
	int numOfSeg_model = 10;
	int numOfSeg_scene = 20;
	int numOfHyp = 20*5;				//broj hipoteza koje dalje evaluiramo
	int numOfShapes = 12;			//broj oblika u bazi
	double tresholdAngle = 30*(PI/280);		//mjera sliènosti kuteva kod provjere kompatibilnosti segmenata
	double tresholdLength = 0.3;	//mjera sliènosti duljina kod provjere kompatibilnosti segmenata
	vector<vector<EdgeSegment>> modelSegments;	//najduži segmenti svih modela
	vector<vector<EdgeSegment>> modelAllSegments;	//svi segmenti svih modela

	ImageIO *io = new JpegIO();
	ColorImage *img = 0;
	
	vector<EdgeSegment> segments;
	vector<EdgeSegment> longestSegs;

	BinaryFilter* bf = new BinaryFilter(150);
	EdgeDetector* ed = new EdgeDetector();
	EdgeSegmentator f;

	/*zakomentirani Tomislavov dio za testiranje
	uèitava sve slike iz foldera baza i sprema binarne slike s oznaèenim segmentima u folder bins	*/
	
	//for (int i = 100; i <= 1300; i++)
	//{
	//	char s[5];
	//	itoa(i, s, 10);
	//	string fileRead(s);
	//	if (fileRead.size() < 4)
	//		fileRead = '0' + fileRead;
	//	fileRead = fileRead + ".jpg";
	//	string fileSave = fileRead;
	//	fileRead = "../baza/" + fileRead;
	//	fileSave = "../bins/" + fileSave;
	//	cout <<"reading " << fileRead <<endl;
	//	img = io->read((char*) fileRead.c_str());
	//	if (img == NULL)
	//		continue;
	//	GrayImage *gImg = bf->applyFilterC2G(img);
	//	delete img;

	//	//EdgeSegmentator f;
	//	//vector<EdgeSegment> segments = f.extractFeatures(gImg, 3);
	//	//ColorImage *cImg = annotate(gImg, segments);
	//	////vector<PixelCoordinates> points;
	//	////points = f.borderFind(gImg);
	//	////ColorImage *cImg = paint(gImg,points);
	//	////io->write(cImg,(char*) fileSaveG.c_str());
	//	
	//	////points.clear();
	//	//segments.clear();
	//	//delete cImg;

	//	io->write(gImg,(char*) fileSave.c_str());
	//	delete gImg;
	//}


	//spremanje segmenata MODELA (primjera za uèenje)
	for (int i = 1; i <= numOfShapes; i++)
	{
		char s[5];
		_itoa_s(i, s, 10);

		string fileRead(s);
		if (i < 10)
			fileRead = '0' + fileRead;

		fileRead = fileRead + "01.jpg";
		imageID = fileRead;
		fileRead = "../baza/" + fileRead;
		cout <<"reading " << fileRead <<endl;
		img = io->read((char*) fileRead.c_str());
		if (img == NULL)
			continue;
		GrayImage *gImg = bf->applyFilterC2G(img);
		delete img;
		EdgeSegmentator f;
		segments = f.extractFeatures(gImg, 3, imageID);
		longestSegs = GetLongestSegs(numOfSeg_scene, segments); 

		modelSegments.push_back(longestSegs);
		modelAllSegments.push_back(segments);
	}
	imageID = "0405";
	string fileNameS = "../baza/0405.jpg";
	string fileNameSexit = "../results/testSrc.jpg";
	char *fileName = (char*)fileNameS.c_str();
	char *fileNameexit = (char*)fileNameSexit.c_str();
	img = io->read(fileName);
	io->write(img, fileNameexit);

	

	GrayImage* fImage = bf->applyFilterC2G(img);
	GrayImage* bImg = ed->binaryConvolution(fImage);
	GrayImage* bsImg = ed->sobel(img,200);
	GrayImage* bfImg = ed->boundaryFollower(fImage);

	
	segments = f.extractFeatures(fImage, 3, imageID);
	
	ColorImage *cImg = annotate(fImage, segments);

	string binImgPath = "../testBin.jpg";
	string boundImgPath = "../Convolution.jpg";
	string boundFolowImgPath = "../Trace.jpg";
	string boundSobelImgPath = "../Sobel.jpg";
	string annImgPath = "../AnnImg.jpg";
	io->write(fImage, (char*)binImgPath.c_str());
	io->write(bImg, (char*)boundImgPath.c_str());
	io->write(bfImg, (char*)boundFolowImgPath.c_str());
	io->write(bsImg, (char*)boundSobelImgPath.c_str());
	io->write(cImg,(char*) annImgPath.c_str());
	
	longestSegs  = GetLongestSegs(numOfSeg_scene, segments); 

	vector<Hypothesis> CompHyps;  //kompatibilne hipoteze
	
	for ( int i = 0; i < longestSegs.size(); i++)
	{
		for ( int j = 0; j < modelSegments.size(); j++)
		{
			for ( int k = 0; k < modelSegments[j].size(); k++)
			{
				Hypothesis hypothesis;

				bool compatible = GenerateErrCovMatrix(hypothesis, modelSegments[j][k], longestSegs[i],tresholdAngle, tresholdLength);

				if (compatible)
				{
					CompHyps.push_back(hypothesis);
				}
			}
		}
	}

	printf("Broj hipoteza: %d\n", CompHyps.size());
	/*for (int i = 0; i < CompHyps.size(); i++) {
		cout << CompHyps[i].getMseg().getImagrID() << endl;
	}*/

	vector<Hypothesis> BestHyps = getBestHyp(numOfHyp, CompHyps); 
	printf("Best hipot: %d\n", BestHyps.size());
	for (int i = 0; i < BestHyps.size(); i++) {
		cout << BestHyps[i].getMseg().getImagrID() << endl;
	}
	/*for (int i = 0; i < BestHyps.size(); i++) {
		string orgSceneFile = "../baza/";
		string orgModelFile = "../baza/";
		orgSceneFile.append(BestHyps[i].getSseg().getImagrID());
		orgModelFile.append(BestHyps[i].getMseg().getImagrID());
		orgSceneFile.append(".jpg");

		cout << "reading: " << orgModelFile << endl;
		ColorImage *orgModelImg = io->read((char*)orgModelFile.c_str());
		GrayImage *gModelImage = bf->applyFilterC2G(orgModelImg);

		cout << "reading: " << orgSceneFile << endl;
		ColorImage *orgSceneImg = io->read((char*)orgSceneFile.c_str());
		GrayImage *gSceneImage = bf->applyFilterC2G(orgSceneImg);

		vector<EdgeSegment> mSegments;
		vector<EdgeSegment> sSegments;

		cout << "Pripremanje segmenata!"<< endl;
		mSegments.push_back(BestHyps[i].getMseg());
		sSegments.push_back(BestHyps[i].getSseg());

		cout << "Anotiranje!"<< endl;
		ColorImage *sAnotImg = annotate(gSceneImage, sSegments);
		ColorImage *mAnotImg = annotate(gModelImage, mSegments);

		char num[10];
		itoa(i, num, 10);

		cout << "Spremanje!" << endl;
		orgSceneFile.append(num);
		orgSceneFile.append(".jpg");
		io->write(sAnotImg, (char*)orgSceneFile.c_str());

		orgModelFile.append(num);
		orgModelFile.append(".jpg");
		io->write(mAnotImg, (char*)orgModelFile.c_str());
		
	}

	//evaluacija hipoteza
	double Qmax = 0.0;
	double iBest = -1;
	for (int i = 0; i < BestHyps.size(); i++)
	{
		
		std::vector<int> matchedScene;
		int j = atoi (BestHyps[i].getMseg().getImagrID().c_str());
		j = (j/100) - 1;
		cout << "\nEvaluating hypothesis: " << i <<" " << modelAllSegments[j][0].getImagrID();
		double Qi = match(BestHyps[i], segments, modelAllSegments[j], matchedScene);
		if (Qi > Qmax)
		{
			iBest = i;
			Qmax = Qi;
		}
		cout << "\nQuality of hypothesis: " << Qi <<"\n";
	}
	cout << "\nBest hypothesis is: " << iBest;
	*/
	return 1;
}
