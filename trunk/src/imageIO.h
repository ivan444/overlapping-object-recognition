#ifndef _IMAGE_IO_H
#define _IMAGE_IO_H

#include "image.h"
#include <string>
#include <vector>

class ImageIO {
public:
	virtual void write(ColorImage* img, string savePath)=0;
	virtual void write(GrayImage* img, string savePath)=0;
	virtual ColorImage& read(string imgPath)=0;
	virtual vector<ColorImage&> readAll(string imgDirPath)=0;
};

#endif

