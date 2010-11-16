#include "edgeDetectors.h"

GrayImage* EdgeDetector::binaryConvolution(GrayImage *img){
	
	GrayImage *boundImg = new GrayImage(img->getWidth(), img->getHeight());
	
	
	GrayPixel pix;
	for(int y = 0; y < img->getHeight(); y++){
		for(int x = 0; x < img->getWidth(); x++){
		
			int upVal, downVal, leftVal, rightVal, currVal;
			if(y > 0) upVal = img->get(x,y-1).v;
			else upVal = OUT_OF_IMAGE;
			if(y < img->getHeight()-1) downVal = img->get(x,y+1).v;
			else downVal = OUT_OF_IMAGE;
			if(x > 0) leftVal = img->get(x-1,y).v;
			else leftVal = OUT_OF_IMAGE;
			if(x < img->getWidth()) rightVal = img->get(x+1,y).v;
			else rightVal = OUT_OF_IMAGE;
			currVal = img->get(x,y).v;
			
			if(currVal == BLACK && upVal+downVal+leftVal+rightVal > 0){
				pix.v = WHITE;
				boundImg->set(x,y,pix);
			}
			
		}
	}
	
	
	return boundImg;
}


GrayImage* EdgeDetector::boundaryFollower(GrayImage *img){
	GrayImage *boundImg = new GrayImage(img->getWidth(), img->getHeight());
	PixelCoordinates white, black;
	bool isOver = false;
	for(int y = 0; y < img->getHeight(); y++){
		for(int x = 1; x < img->getWidth(); x++){
			if(img->get(x-1,y).v == WHITE && img->get(x,y).v == BLACK){
				white.x = x-1; white.y = y;
				black.x = x; black.y = y;
				drawBoundary(img, boundImg, white, black);
				isOver = true;
				break;
			}
			if(isOver) break;
		}
	}
	return boundImg;
}

GrayImage* EdgeDetector::sobel(ColorImage *img, int treshold){

	GrayImage *boundImg = new GrayImage(img->getWidth(), img->getHeight());
	//TODO: ukljuciti rubove
	ColorPixel cp;
	GrayPixel gp;
	for(int y = 1; y < img->getHeight()-1; y++){
		for(int x = 1; x < img->getWidth()-1; x++){
			int a[8] = {0};
			cp = img->get(x-1,y-1);
			a[0] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x,y-1);
			a[1] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x+1,y-1);
			a[2] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x-1,y);
			a[3] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x+1,y);
			a[4] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x-1,y+1);
			a[5] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x,y+1);
			a[6] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			cp = img->get(x+1,y+1);
			a[7] = (int)(cp.v1*RED_F + cp.v2*GREEN_F + cp.v3*BLUE_F);
			int sx = a[2] + 2*a[4] + a[7] - a[0] - 2*a[3] - a[5];
			int sy = a[0] + 2*a[1] + a[2] - a[5] - 2*a[6] - a[7];
			int M = (int)abs((double)sx) + abs((double)sy);
			if(M > treshold) gp.v = WHITE;
			else gp.v = BLACK;
			boundImg->set(x,y,gp);
		}
	}
	return boundImg;
	

}

void EdgeDetector::drawBoundary(GrayImage *origImg, GrayImage *boundImg, 
						PixelCoordinates moon, PixelCoordinates planet){
	int xOffset[8] = {-1,-1,0,1,1,1,0,-1};
	int yOffset[8] = {0,-1,-1,-1,0,1,1,1};
	int offsetIter = 0;
	GrayPixel white;
	white.v = WHITE;
	boundImg->set(planet.x, planet.y, white);
	PixelCoordinates start;
	start.x = planet.x;
	start.y = planet.y;
	for(int k = 0; k < origImg->getWidth()*origImg->getHeight(); k++){
		for(int i = 1; i < 8; i++){
			int orbitX = (planet.x+xOffset[(offsetIter+i)%8])%boundImg->getWidth();
			int orbitY = (planet.y+yOffset[(offsetIter+i)%8])%boundImg->getHeight();
			if(origImg->get(orbitX, orbitY).v == BLACK){
				planet.x = orbitX;
				planet.y = orbitY;
				boundImg->set(planet.x, planet.y, white);
				offsetIter = ((int)((offsetIter+i)/2)+3)%4*2;
				break;
			}

		}
		if(start.x == planet.x && start.y == planet.y) break;

	}
}
 
