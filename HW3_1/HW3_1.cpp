#include "HW3_1.hpp"

void HW3_1_1()
{
	Image img;
	img.open_img("2_b.raw", ImageSize(512, 512));
	img.blur(3);
	img.nearest(ImageSize(256, 256));
	img.threshold(130);
	img.save_img("hw3_1_1.raw", true);
}

void HW3_1_2()
{
	Image doraemon;
	doraemon.open_img("2_b.raw", ImageSize(512, 512));
	doraemon.blur(3);
	doraemon.nearest(ImageSize(256, 256));
	doraemon.threshold(130);

	Image lena256;
	lena256.open_img("lena256.raw", ImageSize(256, 256));
	lena256.replaceBitPlane(doraemon, 0);
	lena256.save_img("hw3_1_2_0-bit.raw", true);

	Image img_modified;
	img_modified.open_img("hw3_1_2_0-bit.raw", ImageSize(256, 256));
	img_modified.extractBitPlane(0);
	img_modified.save_img("hw3_1_2_doraemon.raw", true);
}

void HW3_1_3()
{
	Image doraemon;
	doraemon.open_img("2_b.raw", ImageSize(512, 512));
	doraemon.blur(3);
	doraemon.nearest(ImageSize(256, 256));
	doraemon.threshold(130);

	Image lena256;
	lena256.open_img("lena256.raw", ImageSize(256, 256));

	Image img_modified;
	for (int bit = 1; bit <= 7; bit++)
	{
		stringstream filename;
		filename << "hw3_1_3_" << bit << "-bit.raw";
		img_modified = lena256;
		img_modified.replaceBitPlane(doraemon, bit);
		img_modified.save_img(filename.str().c_str(), true);
	}
}
