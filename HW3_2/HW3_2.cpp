#include "HW3_2.hpp"

void HW3_2_1()
{
	Image log512;
	log512.open_img("log512.raw", ImageSize(512, 512));

	Image img;
	img = log512;
	img.logTrans();
	img.save_img("hw3_2_1_Log.raw", true);
	img = log512;
	img.invLogTrans();
	img.save_img("hw3_2_1_invLog.raw", true);
}

void HW3_2_2()
{
	Image log512;
	log512.open_img("log512.raw", ImageSize(512, 512));

	double gamma[4] = { 0.1, 0.4, 2.5, 10 };
	for (int i = 0; i < 4; i++)
	{
		stringstream filename;
		filename << "hw3_2_2_" << (int)(gamma[i] * 10) << ".raw";
		Image img(log512);
		img.gammaTrans(gamma[i]);
		img.save_img(filename.str().c_str(), true);
	}
}

void HW3_2_3()
{
	Image log512_Neg;
	log512_Neg.open_img("log512.raw", ImageSize(512, 512));
	log512_Neg.negative();
	log512_Neg.save_img("hw3_2_3_Neg.raw", true);

	Image img;
	img = log512_Neg;
	img.logTrans();
	img.save_img("hw3_2_3_Log.raw", true);
	img = log512_Neg;
	img.invLogTrans();
	img.save_img("hw3_2_3_invLog.raw", true);

	double gamma[4] = { 0.1, 0.4, 2.5, 10 };
	for (int i = 0; i < 4; i++)
	{
		stringstream filename;
		filename << "hw3_2_3_" << (int)(gamma[i] * 10) << ".raw";
		img = log512_Neg;
		img.gammaTrans(gamma[i]);
		img.save_img(filename.str().c_str(), true);
	}
}

