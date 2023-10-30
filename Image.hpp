#ifndef _IMAGE_
#define _IMAGE_

#include <iostream>
#include <vector>
using namespace std;

#include <cstdio>
#include <cstdint>
#include <cmath>

typedef union
{
	uint8_t pixel;
	struct
	{
		unsigned b0 : 1;
		unsigned b1 : 1;
		unsigned b2 : 1;
		unsigned b3 : 1;
		unsigned b4 : 1;
		unsigned b5 : 1;
		unsigned b6 : 1;
		unsigned b7 : 1;
	};
} BitPlane;

class ImageSize
{
public:
	int rows;
	int columns;
	ImageSize();
	ImageSize(int rows, int columns);
	ImageSize(const ImageSize& source);
	int size();
	ImageSize operator=(const ImageSize& other);
	ImageSize operator*(const double& other);
	ImageSize operator/(const double& other);
	bool operator==(const ImageSize& other);
	bool operator!=(const ImageSize& other);
};

class Coordinate
{
public:
	int row;
	int column;
	Coordinate();
	Coordinate(int row, int column);
	Coordinate operator=(const Coordinate& other);
	bool operator==(const Coordinate& other);
	bool operator!=(const Coordinate& other);
};

class Image
{
public:
	ImageSize size;
	vector<vector<uint8_t>> img;
	void open_img(const char* FileName, ImageSize size);
	void save_img(const char* FileName, bool printName);
	void set_img(const Image& source);
	void blur(int mask);
	void nearest(ImageSize size_new);
	void bicubic(ImageSize size_new);
	void threshold(uint8_t th);
	void replaceBitPlane(Image& img_in, int bit);
	void extractBitPlane(int bit);
	void logTrans();
	void invLogTrans();
	void gammaTrans(double gamma);
	void negative();
	void histEqualize();

	Image();
	Image(const Image& source);
	Image(ImageSize size, uint8_t value);
	Image(vector<vector<uint8_t>>& img);
	Image operator=(const Image& other);

protected:
	vector<vector<uint8_t>> padding(int pad);
	double bicubic_param(double x, double a);
};

#endif /* _IMAGE_ */