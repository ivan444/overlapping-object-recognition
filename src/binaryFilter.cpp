/*
 * Filter za binarizaciju slika jednostrukim pragom.
 * Koristi se konstantni prag definiran izvana.
 */

#include "binaryFilter.h"

BinaryFilter::BinaryFilter(int threshold) : _threshold(threshold) {}


//ColorImage* applyFilterC2C(ColorImage *src)=0;
GrayImage* BinaryFilter::applyFilterC2G(ColorImage *src) {
	GrayImage* filtered = new GrayImage(src->getWidth(), src->getHeight());

	// ColorImage to GrayImage with threshold
	int w = src->getWidth();
	int h = src->getHeight();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			ColorPixel cp = src->get(x,y);
			GrayPixel gp;
			gp.v = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			if (gp.v > _threshold) gp.v = 0;
			else gp.v = 255;
			filtered->set(x,y,gp);
		}
	}

	return filtered;
}

GrayImage* BinaryFilter::applyFilterG2G(GrayImage *src) {
	GrayImage* filtered = new GrayImage(src->getWidth(), src->getHeight());

	// ColorImage to GrayImage with threshold
	int w = src->getWidth();
	int h = src->getHeight();
	for (int y = 0; y < h; y++) {
		for (int x = 0; x < w; x++) {
			GrayPixel gp = src->get(x,y);
			if (gp.v < _threshold) gp.v = 0;
			else gp.v = 255;
			filtered->set(x,y,gp);
		}
	}

	return filtered;
}
