#ifndef _FILTER_H
#define _FILTER_H

#include "image.h"

class Filter {
public:
	//virtual ColorImage* applyFilterC2C(ColorImage *src)=0;
	virtual GrayImage* applyFilterC2G(ColorImage *src)=0;
	virtual GrayImage* applyFilterG2G(GrayImage *src)=0;
};

#endif
