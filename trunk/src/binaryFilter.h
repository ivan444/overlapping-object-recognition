#ifndef _BINARY_FILTER_H
#define _BINARY_FILTER_H

#include "filter.h"

// Faktori za pretvorbu color -> gray
#define RED_F 0.3
#define GREEN_F 0.59
#define BLUE_F 0.11

class BinaryFilter : public Filter {
private:
	int _threshold;

public:
	BinaryFilter(int threshold);
	GrayImage* applyFilterC2G(ColorImage *src);
	GrayImage* applyFilterG2G(GrayImage *src);
};

#endif