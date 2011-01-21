#include "annotate.h"
#include <cmath>

using namespace std;

ColorImage* annotate(GrayImage *gImg, vector<PixelCoordinates> &points)
{
	int width = gImg->getWidth();
	int height = gImg->getHeight();
	ColorImage * cImg = new ColorImage(width, height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			ColorPixel pix;
			pix.v1 = pix.v2 = pix.v3 = gImg->get(j,i).v;
			cImg->set(j, i, pix);
		}
	}

	int size = points.size();
	for (int i = 0; i < size; i++)
	{
		ColorPixel pix;
		pix.v1 = 255;
		pix.v2 = pix.v3 = 0;
		cImg->set(points[i].x, points[i].y, pix);
	}
	return cImg;

}

ColorImage* annotate(GrayImage *gImg, vector<EdgeSegment> &lines)
{
	int width = gImg->getWidth();
	int height = gImg->getHeight();
	ColorImage * cImg = new ColorImage(width, height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j<width; j++)
		{
			ColorPixel pix;
			pix.v1 = pix.v2 = pix.v3 = gImg->get(j,i).v;
			cImg->set(j, i, pix);
		}
	}
	for (int i = 0; i < lines.size(); i++)
	{

		int x0 = lines[i].getFirst().x;
		int y0 = lines[i].getFirst().y;
		int x1 = lines[i].getLast().x;
		int y1 = lines[i].getLast().y;

		ColorPixel edge;
		edge.v2 = edge.v3 = 0;
		edge.v1 = 255;
		
		for (int dx = -4; dx <= 4; dx++) {
			for (int dy = -4; dy <= 4; dy++) {
				cImg->set(x0+dx,y0+dy,edge);
				cImg->set(x1+dx,y1+dy,edge);
			}
		}
		
		// Negdje je greska... ovo je quick fix:
		if ((x0 < 0 || x0 > width)
			|| (x1 < 0 || x1 > width)
			|| (y0 < 0 || y0 > height)
			|| (y1 < 0 || y1 > height)) {
				continue;
		}

		bool steep = 0;
		if ( (abs(y1-y0)) > (abs(x1-x0)))
		{
			steep = 1;
			int h = x0; x0 = y0; y0 = h;
			h = x1;
			x1 = y1;
			y1 = h; 
		}
		if (x0 > x1 )
		{
			int h = x0; x0 = x1; x1 = h;
			h = y0; y0 =y1; y1 = h;
		}
		int deltax = x1-x0;
		int deltay = abs(y1-y0);
		int error = deltax/2;
		int ystep;
		int y = y0;
		if (y0 < y1)
			ystep = 1;
		else
			ystep = -1;
		int x;
		//error = error*ystep;
		for ( x = x0; x <= x1; x++)
		{
			ColorPixel pix;
			pix.v1 = pix.v3 = 0;
			pix.v2 = 255;
			if (steep)
				cImg->set(y,x,pix);
			else
				cImg-> set(x,y,pix);
			error = error - deltay;
			
			if (error <0)
			{
				y = y+ystep;
				error = error+deltax;
			}
		}
		x = x;
	}

	/*
function line(x0, x1, y0, y1)
     boolean steep := abs(y1 - y0) > abs(x1 - x0)
     if steep then
         swap(x0, y0)
         swap(x1, y1)
     if x0 > x1 then
         swap(x0, x1)
         swap(y0, y1)
     int deltax := x1 - x0
     int deltay := abs(y1 - y0)
     int error := deltax / 2
     int ystep
     int y := y0
     if y0 < y1 then ystep := 1 else ystep := -1
     for x from x0 to x1
         if steep then plot(y,x) else plot(x,y)
         error := error - deltay
         if error < 0 then
             y := y + ystep
             error := error + deltax
*/
	return cImg;
}


