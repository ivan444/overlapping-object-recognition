
#include "openCVJpegIO.h"

void JpegIO::write(ColorImage* img, char* savePath){
	//cout << "Pisem " << savePath << endl;
	IplImage * pRGBImg  = 0;
	pRGBImg = cvCreateImage
   ( cvSize(img->getWidth(),img->getHeight() ), 
   IPL_DEPTH_8U, RGB_IMAGE_CHANNELS );
   copyImgToIplColor(pRGBImg, img);
   
   
   if( !cvSaveImage(savePath, pRGBImg) ){
		cerr << "Failed to write image file.\n";
		return;
	}
	cvReleaseImage(&pRGBImg);
	//cout << "Gotovo pisanje " << savePath << endl;
	return;
}

void JpegIO::write(GrayImage* img, char* savePath){
	
	IplImage * pGrayImg  = 0;
	pGrayImg = cvCreateImage
   ( cvSize(img->getWidth(),img->getHeight() ), 
   IPL_DEPTH_8U, GRAY_IMAGE_CHANNELS );
   copyImgToIplGray(pGrayImg, img);
   if( !cvSaveImage(savePath, pGrayImg) ){
		cerr << "Failed to write image file." << endl;
		return;
	}
	cvReleaseImage(&pGrayImg);
	return;
}

ColorImage* JpegIO::read(char* imgPath){
	  //cout << "Ucitavam " << imgPath << endl;
	  IplImage * pRGBImg  = 0;

	  // Load the RGB image from file
	  pRGBImg = cvLoadImage(imgPath, CV_LOAD_IMAGE_UNCHANGED);
	  if(!pRGBImg){
	  	cerr << "Failed to load input image." << endl;
		return NULL;
	  }
	  ColorImage *img = new ColorImage(pRGBImg->width, pRGBImg->height);
	  copyIplToImgColor(pRGBImg, img);
	  //cvNamedWindow("Color test 1");
     //cvShowImage("Color test 1", pRGBImg);
     //cvWaitKey(0);
	 // cvReleaseImage(&pRGBImg);
	  //cout << "Gotovo ucitavanje " << imgPath << endl;
	  return img;
}

void JpegIO::copyImgToIplColor(IplImage *iplImg, ColorImage *img){
	
	int red=0, green=0, blue=0;
	for(int x = 0; x < img->getWidth(); x++){
		for(int y = 0; y < img->getHeight(); y++){
			
			red = img->get(x,y).v1;
			green = img->get(x,y).v2;
			blue = img->get(x,y).v3;
			/*
			*TODO: pogledati kako obaviti ovo castanje iz int u uchar.
			*		 možda promijeniti tip varijable ColorPixel.v*
			*/
			//cout << "colourtest 1. red= " << red << ", green=" << green << "blue=" << blue << endl;
			((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3] = (uchar)blue;
			((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3+1] = (uchar)green;
			((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3+2] = (uchar)red;
		}
	}
	return;
}

void JpegIO::copyImgToIplGray(IplImage *iplImg, GrayImage *img){
	
	int value = 0;
	for(int x = 0; x < img->getWidth(); x++){
		for(int y = 0; y < img->getHeight(); y++){
			
			value = img->get(x,y).v;
			/*
			*TODO: pogledati kako obaviti ovo castanje iz int u uchar.
			*		 možda promijeniti tip varijable GrayPixel.v
			*/
			((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x] = (uchar)value;
		}
	}
	return;
}

void JpegIO::copyIplToImgColor(IplImage *iplImg, ColorImage *img){
	int red = 0, green = 0, blue = 0;
	
	ColorPixel pixel;
	for(int x = 0; x < img->getWidth(); x++){
		for(int y = 0; y < img->getHeight(); y++){
			red = ((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3+2];
			green = ((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3+1];
			blue = ((uchar*)(iplImg->imageData + iplImg->widthStep*y))[x*3];
			pixel.v1 = red;
			pixel.v2 = green;
			pixel.v3 = blue;
			img->set(x,y,pixel);
		}
	}

	return;
}
