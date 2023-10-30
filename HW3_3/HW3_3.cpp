#include "HW3_3.hpp"

void Histogram::histMatch(vector<double>& hist)
{
	// G[z]
	vector<uint8_t> G(256, 0);
	double sum = 0;
	for (int i = 0; i < 256; i++)
	{
		sum += hist[i];
		G[i] = (uint8_t)round(sum * 255);
	}

	// G_inv[s]
	vector<uint8_t> G_inv(256, 0);
	for (int s = 0; s < 256; s++)
	{
		bool find = false;
		for (int i = 0; i < 256; i++)
			if (G[i] == s)
			{
				G_inv[s] = i;
				find = true;
				break;
			}

		if (find == false && s > 0)
			G_inv[s] = G_inv[s - 1];
	}

	// T[r]
	Histogram::histEqualize();

	// G_inv[T[r]]
	for (auto& i : Histogram::img)
		for (auto& j : i)
			j = G_inv[j];
}

void Histogram::plotHistogram()
{
	vector<int> hist(256, 0);

	for (auto& i : Histogram::img)
		for (auto& j : i)
			hist[j] += 1;

	int max = *max_element(hist.begin(), hist.end());
	for (auto& i : hist)
		i = (int)(i / (double)max * 380);

	Mat hist_img(Size(576, 480), CV_8UC1);
	hist_img.setTo(255);
	rectangle(hist_img, Rect(30, 30, 517, 402), Scalar(0));
	line(hist_img, Point(33, 431), Point(33, 437), Scalar(0));
	putText(hist_img, "0", Point(28, 450), FONT_HERSHEY_PLAIN, 1, Scalar(0), 1);
	line(hist_img, Point(187, 431), Point(187, 437), Scalar(0));
	putText(hist_img, "77", Point(177, 450), FONT_HERSHEY_PLAIN, 1, Scalar(0), 1);
	line(hist_img, Point(339, 431), Point(339, 437), Scalar(0));
	putText(hist_img, "153", Point(325, 450), FONT_HERSHEY_PLAIN, 1, Scalar(0), 1);
	line(hist_img, Point(543, 431), Point(543, 437), Scalar(0));
	putText(hist_img, "255", Point(529, 450), FONT_HERSHEY_PLAIN, 1, Scalar(0), 1);
	putText(hist_img, "Grayscale Value (0 to 255)", Point(177, 470), FONT_HERSHEY_PLAIN, 1, Scalar(0), 1);
	for (int i = 0; i <= 255; i++)
		line(hist_img, Point(33 + i * 2, 431), Point(33 + i * 2, 431 - hist[i]), Scalar(0));

	Histogram::sizeHist = ImageSize(480, 576);
	Histogram::img_Hist = vector<vector<uint8_t>>(480, vector<uint8_t>(576, 0));
	for (int r = 0; r < 480; r++)
		for (int c = 0; c < 576; c++)
			Histogram::img_Hist[r][c] = hist_img.at<uchar>(r, c);
}

void Histogram::saveHistogram(const char* FileName, bool printName)
{
	uint8_t* buffer = new uint8_t[Histogram::sizeHist.size()];

	for (int r = 0; r < Histogram::sizeHist.rows; r++)
		for (int c = 0; c < Histogram::sizeHist.columns; c++)
			buffer[c + r * Histogram::sizeHist.columns] = Histogram::img_Hist[r][c];

	FILE* output_file;
	fopen_s(&output_file, FileName, "wb");
	fwrite(buffer, sizeof(uint8_t), Histogram::sizeHist.size(), output_file);

	if (printName == true)
		cout << "Output file: " << FileName << endl;

	delete[] buffer;
	fclose(output_file);
}

void HW3_3_1()
{
	Histogram log512;
	log512.open_img("log512.raw", ImageSize(512, 512));
	log512.plotHistogram();
	log512.saveHistogram("hw3_3_1_hist.raw", true);
	log512.negative();
	log512.plotHistogram();
	log512.saveHistogram("hw3_3_1_Neg_hist.raw", true);
}

void HW3_3_2()
{
	Image log512;
	log512.open_img("log512.raw", ImageSize(512, 512));

	Histogram hist;
	hist.set_img(log512);
	hist.histEqualize();
	hist.save_img("hw3_3_2_histEq.raw", true);
	hist.plotHistogram();
	hist.saveHistogram("hw3_3_2_hist.raw", true);

	hist.set_img(log512);
	hist.negative();
	hist.histEqualize();
	hist.save_img("hw3_3_2_Neg_histEq.raw", true);
	hist.plotHistogram();
	hist.saveHistogram("hw3_3_2_Neg_hist.raw", true);
}

void HW3_3_3()
{
	Histogram log512;
	log512.open_img("log512.raw", ImageSize(512, 512));

	vector<double> hist(256, 0);
	double sum = 0;
	for (int i = 77; i <= 153; i++)
	{
		hist[i] = 1;
		sum++;
	}
	for (int i = 0; i < 256; i++)
		hist[i] = hist[i] / sum;

	log512.histMatch(hist);
	log512.save_img("hw3_3_3_histMatch.raw", true);
	log512.plotHistogram();
	log512.saveHistogram("hw3_3_3_hist.raw", true);
}
