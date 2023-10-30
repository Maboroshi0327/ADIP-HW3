#ifndef _HW3_3_
#define _HW3_3_

#include <vector>
#include <algorithm>
using namespace std;

#include <opencv2\opencv.hpp>
using namespace cv;

#include <sstream>
#include <cstdint>
#include "../Image.hpp"

class Histogram : public Image
{
public:
	ImageSize sizeHist;
	vector<vector<uint8_t>> img_Hist;
	void histMatch(vector<double>& hist);
	void plotHistogram();
	void saveHistogram(const char* FileName, bool printName);
};

void HW3_3_1();
void HW3_3_2();
void HW3_3_3();

#endif /* _HW3_3_ */
