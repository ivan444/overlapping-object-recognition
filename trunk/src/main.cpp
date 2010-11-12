#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"
#include "binaryFilter.h"
#include "edgeDetectors.h"

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
	GrayImage* bfImg = ed->boundaryFolower(fImage);
	string binImgPath = "../testBin.jpg";
	string boundImgPath = "../Convolution.jpg";
	string boundFolowImgPath = "../Trace.jpg";
	string boundSobelImgPath = "../Sobel.jpg";
	io->write(fImage, (char*)binImgPath.c_str());
	io->write(bImg, (char*)boundImgPath.c_str());
	io->write(bfImg, (char*)boundFolowImgPath.c_str());
	io->write(bsImg, (char*)boundSobelImgPath.c_str());
	return 1;
}
