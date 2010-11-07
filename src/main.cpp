#include <iostream>
#include <string>
#include "imageIO.h"
#include "openCVJpegIO.h"

using namespace std;

int main() {
	cout << "Hello\n";
	ImageIO *io = new JpegIO();
	ColorImage *img = 0;
	string fileNameS = "lena.jpg";
	string fileNameSexit = "testSrc.jpg";
	char *fileName = (char*)fileNameS.c_str();
	char *fileNameexit = (char*)fileNameSexit.c_str();
	img = io->read(fileName);
	io->write(img, fileNameexit);
	return 1;
}
