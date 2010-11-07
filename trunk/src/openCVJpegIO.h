#ifndef _OPENCV_JPEG_IO_H
#define _OPENCV_JPEG_IO_H

#include "imageIO.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
#define RGB_IMAGE_CHANNELS 3
#define GRAY_IMAGE_CHANNELS 1
#define STANDARD_IMAGE_DEPTH 8 //8 bytes (uchar)

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