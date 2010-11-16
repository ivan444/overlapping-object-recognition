#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "binaryFilter.h"
#include "edgeDetectors.h"
#include "edgeSegmentator.h"
#include "annotate.h"

using namespace std;

int main() {
	
	cout << "Hello\n";
	cout << (-1%3) << endl;
	
	
	ImageIO *io = new JpegIO();
	ColorImage *img = 0;
	string fileNameS = "../0907.jpg";
	string fileNameSexit = "../testSrc.jpg";
	char *fileName = (char*)fileNameS.c_str();
	char *fileNameexit = (char*)fileNameSexit.c_str();
	img = io->read(fileName);
	io->write(img, fileNameexit);

	BinaryFilter* bf = new BinaryFilter(150);
	EdgeDetector* ed = new EdgeDetector();
	GrayImage* fImage = bf->applyFilterC2G(img);
	GrayImage* bImg = ed->binaryConvolution(fImage);
	GrayImage* bsImg = ed->sobel(img,200);
	GrayImage* bfImg = ed->boundaryFollower(fImage);
	string binImgPath = "../testBin.jpg";
	string boundImgPath = "../Convolution.jpg";
	string boundFolowImgPath = "../Trace.jpg";
	string boundSobelImgPath = "../Sobel.jpg";
	io->write(fImage, (char*)binImgPath.c_str());
	io->write(bImg, (char*)boundImgPath.c_str());
	io->write(bfImg, (char*)boundFolowImgPath.c_str());
	io->write(bsImg, (char*)boundSobelImgPath.c_str());


	//zakomentirani Tomislavov dio za testiranje
	//uèitava sve slike iz foldera baza i sprema binarne slike s oznaèenim segmentima u folder bins
	/*JpegIO io;
	ColorImage *img;
	BinaryFilter bf(141);
	
	for (int i = 100; i <= 1300; i++)
	{
		char s[5];
		itoa(i, s, 10);
		string fileRead(s);
		if (fileRead.size() < 4)
			fileRead = '0' + fileRead;
		fileRead = fileRead + ".jpg";
		string fileSave = fileRead;
		fileRead = "../baza/" + fileRead;
		fileSave = "../bins/" + fileSave;
		cout <<"reading " << fileRead <<endl;
		img = io.read((char*) fileRead.c_str());
		if (img == NULL)
			continue;
		GrayImage *gImg = bf.applyFilterC2G(img);
		delete img;
		EdgeSegmentator f;
		vector<EdgeSegment> segments = f.extractFeatures(gImg, 3);
		ColorImage *cImg = paint (gImg, segments);
		//vector<PixelCoordinates> points;
		//points = f.borderFind(gImg);
		//ColorImage *cImg = paint(gImg,points);
		io.write(cImg,(char*) fileSave.c_str());
		//points.clear();
		segments.clear();
		delete gImg;
		delete cImg;
	}*/

	return 1;
}
