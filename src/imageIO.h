#ifndef _IMAGE_IO_H
#define _IMAGE_IO_H


#include "image.h"
#include <string>
#include <vector>
#include <iostream>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#define RGB_IMAGE_CHANNELS 3
#define GRAY_IMAGE_CHANNELS 1
#define STANDARD_IMAGE_DEPTH 8 //8 bytes (uchar)
using namespace std;
class ImageIO {
public:
	virtual void write(ColorImage* img, char* savePath)=0;
	virtual void write(GrayImage* img, char* savePath)=0;
	virtual ColorImage* read(char* imgPath)=0;
	//virtual vector<ColorImage&> readAll(string imgDirPath)=0;
};

class JpegIO : public ImageIO {

public:
	virtual void write(ColorImage* img, char* savePath);
	virtual void write(GrayImage* img, char* savePath);
	virtual ColorImage* read(char* imgPath);
	//virtual vector<ColorImage&> readAll(string imgDirPath);
private:
	void copyImgToIplColor(IplImage *iplImg, ColorImage *img);
	void copyImgToIplGray(IplImage *iplImg, GrayImage *img);
	void copyIplToImgColor(IplImage *iplImg, ColorImage *img);
	void copyIplToImgGray(IplImage *iplImg, ColorImage *img);
};


#endif

