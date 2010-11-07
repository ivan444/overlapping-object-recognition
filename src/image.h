#ifndef _IMAGE_H
#define _IMAGE_H

/**
 * Pixel sa tri vrijednosti za slike u boji (npr. RGB ili HSI pixel).
 */
struct ColorPixel {
	int v1;				//R ili H
	int v2;				//G ili S
	int v3;				//B ili I
};

/**
 * Pixel sa jednom vrijednošću (npr. grayscale pixel).
 */
struct GrayPixel {
	int v;
};

class ColorImage {
private:
	int width;
	int height;
	ColorPixel* imgData;
public:
	ColorImage(int width, int height) {
		this->width = width;
		this->height = height;
		int imgSize = width*height;
		imgData = new ColorPixel[imgSize];
	}

	~ColorImage() {
		delete[] imgData;
	}

	/**
	 * Dohvat pixela - koristi se lijevo orjentirani
	 * koordinatni sustav (kao kod matrica).
	 */ 
	ColorPixel get(int x, int y) {
		return imgData[y*width+x];
	}

	/**
	 * Postavljanje pixela - koristi se lijevo orjentirani
	 * koordinatni sustav (kao kod matrica).
	 */ 
	void set(int x, int y, ColorPixel val) {
		imgData[y*width+x] = val;
	}
	
	int getWidth(){
		return this->width;
	}
	
	int getHeight(){
		return this->height;
	}
	
};

class GrayImage {
private:
	int width;
	int height;
	GrayPixel* imgData;
public:
	// TODO: Napisati copy-constructor!!
	GrayImage(int width, int height) {
		this->width = width;
		this->height = height;
		int imgSize = width*height;
		imgData = new GrayPixel[imgSize];
	}

	~GrayImage() {
		delete[] imgData;
	}

	/**
	 * Dohvat pixela - koristi se lijevo orjentirani
	 * koordinatni sustav (kao kod matrica).
	 * TODO: iskoristiti operator overload!!
	 */ 
	GrayPixel get(int x, int y) {
		return imgData[y*width+x];
	}

	/**
	 * Postavljanje pixela - koristi se lijevo orjentirani
	 * koordinatni sustav (kao kod matrica).
	 * TODO: iskoristiti operator overload!!
	 */ 
	void set(int x, int y, GrayPixel val) {
		imgData[y*width+x] = val;
	}
	
	int getWidth(){
		return this->width;
	}
	
	int getHeight(){
		return this->height;
	}
	
};

#endif

