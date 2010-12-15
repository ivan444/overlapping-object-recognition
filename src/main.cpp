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
	
	int numOfSeg_model = 10;
	int numOfSeg_scene = 20;
	int numOfShapes = 12;			//broj oblika u bazi
	double tresholdAngle = 30;		//mjera sli�nosti kuteva kod provjere kompatibilnosti segmenata

	vector<vector<EdgeSegment>> sceneSegments;	//segmenti scene

	ImageIO *io = new JpegIO();
	ColorImage *img = 0;
	
	vector<EdgeSegment> segments;
	vector<EdgeSegment> longestSegs;

	BinaryFilter* bf = new BinaryFilter(150);
	EdgeDetector* ed = new EdgeDetector();
	EdgeSegmentator f;

	/*zakomentirani Tomislavov dio za testiranje
	u�itava sve slike iz foldera baza i sprema binarne slike s ozna�enim segmentima u folder bins	*/
	
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


	//spremanje segmenata scene (primjera za u�enje)
	for (int i = 1; i <= numOfShapes; i++)
	{
		char s[5];
		itoa(i, s, 10);

		string fileRead(s);
		if (i < 10)
			fileRead = '0' + fileRead;

		fileRead = fileRead + "01.jpg";
		fileRead = "../baza/" + fileRead;
		cout <<"reading " << fileRead <<endl;
		img = io->read((char*) fileRead.c_str());
		if (img == NULL)
			continue;
		GrayImage *gImg = bf->applyFilterC2G(img);
		delete img;
		EdgeSegmentator f;
		segments = f.extractFeatures(gImg, 3);
		longestSegs = GetLongestSegs(numOfSeg_scene, segments); 

		sceneSegments.push_back(longestSegs);
	}

	string fileNameS = "../baza/0102.jpg";
	string fileNameSexit = "../results/testSrc.jpg";
	char *fileName = (char*)fileNameS.c_str();
	char *fileNameexit = (char*)fileNameSexit.c_str();
	img = io->read(fileName);
	io->write(img, fileNameexit);

	

	GrayImage* fImage = bf->applyFilterC2G(img);
	GrayImage* bImg = ed->binaryConvolution(fImage);
	GrayImage* bsImg = ed->sobel(img,200);
	GrayImage* bfImg = ed->boundaryFollower(fImage);


	segments = f.extractFeatures(fImage, 3);
	
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





	return 1;
}
