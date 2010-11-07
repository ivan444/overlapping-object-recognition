#ifndef _IMAGE_IO_H
#define _IMAGE_IO_H

#include "image.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;
class ImageIO {
public:
	virtual void write(ColorImage* img, char* savePath)=0;
	virtual void write(GrayImage* img, char* savePath)=0;
	virtual ColorImage* read(char* imgPath)=0;
	//virtual vector<ColorImage&> readAll(string imgDirPath)=0;
};

#endif

