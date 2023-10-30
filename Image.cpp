#include "Image.hpp"

ImageSize::ImageSize()
{
	ImageSize::rows = 0;
	ImageSize::columns = 0;
}

ImageSize::ImageSize(int rows, int columns)
{
	ImageSize::rows = rows;
	ImageSize::columns = columns;
}

ImageSize::ImageSize(const ImageSize& source)
{
	ImageSize::rows = source.rows;
	ImageSize::columns = source.columns;
}

int ImageSize::size()
{
	return ImageSize::rows * ImageSize::columns;
}

ImageSize ImageSize::operator=(const ImageSize& other)
{
	ImageSize::rows = other.rows;
	ImageSize::columns = other.columns;
	return *this;
}

ImageSize ImageSize::operator*(const double& other)
{
	return ImageSize(int(ImageSize::rows * other), int(ImageSize::columns * other));
}

ImageSize ImageSize::operator/(const double& other)
{
	return ImageSize(int(ImageSize::rows / other), int(ImageSize::columns / other));
}

bool ImageSize::operator==(const ImageSize& other)
{
	bool result = ImageSize::rows == other.rows;
	result &= ImageSize::columns == other.columns;
	return result;
}

bool ImageSize::operator!=(const ImageSize& other)
{
	bool result = ImageSize::rows != other.rows;
	result |= ImageSize::columns != other.columns;
	return result;
}

Coordinate::Coordinate()
{
	Coordinate::row = 0;
	Coordinate::column = 0;
}

Coordinate::Coordinate(int row, int column)
{
	Coordinate::row = row;
	Coordinate::column = column;
}

Coordinate Coordinate::operator=(const Coordinate& other)
{
	Coordinate::row = other.row;
	Coordinate::column = other.column;
	return *this;
}

bool Coordinate::operator==(const Coordinate& other)
{
	bool result = Coordinate::row == other.row;
	result &= Coordinate::column == other.column;
	return result;
}

bool Coordinate::operator!=(const Coordinate& other)
{
	bool result = Coordinate::row != other.row;
	result |= Coordinate::column != other.column;
	return result;
}

void Image::open_img(const char* FileName, ImageSize size)
{
	uint8_t* buffer = new uint8_t[size.size()];

	FILE* input_file;
	if (fopen_s(&input_file, FileName, "rb") != 0)
	{
		printf("Couldn't find file %s.\n", FileName);
		exit(1);
	}
	fread_s(buffer, size.size(), sizeof(uint8_t), size.size(), input_file);

	Image::size = size;
	Image::img = vector<vector<uint8_t>>(size.rows, vector<uint8_t>(size.columns, 0));
	for (int r = 0; r < size.rows; r++)
		for (int c = 0; c < size.columns; c++)
			Image::img[r][c] = buffer[c + r * size.columns];

	delete[] buffer;
	fclose(input_file);
}

void Image::save_img(const char* FileName, bool printName)
{
	uint8_t* buffer = new uint8_t[Image::size.size()];

	for (int r = 0; r < Image::size.rows; r++)
		for (int c = 0; c < Image::size.columns; c++)
			buffer[c + r * Image::size.columns] = Image::img[r][c];

	FILE* output_file;
	fopen_s(&output_file, FileName, "wb");
	fwrite(buffer, sizeof(uint8_t), Image::size.size(), output_file);

	if (printName == true)
		cout << "Output file: " << FileName << endl;

	delete[] buffer;
	fclose(output_file);
}

void Image::set_img(const Image& source)
{
	Image::size = source.size;
	Image::img = source.img;
}

void Image::blur(int mask)
{
	int pad = mask / 2;
	vector<vector<uint8_t>> img_pad = Image::padding(pad);

	for (int r = pad; r < Image::size.rows + pad; r++)
	{
		for (int c = pad; c < Image::size.columns + pad; c++)
		{
			int sum = 0;
			for (int i = -pad; i <= pad; i++)
			{
				for (int j = -pad; j <= pad; j++)
				{
					sum = sum + img_pad[r + i][c + j];
				}
			}
			img[r - pad][c - pad] = sum / mask / mask;
		}
	}
}

void Image::nearest(ImageSize size_new)
{
	ImageSize size_old(Image::size);
	vector<vector<uint8_t>> img_new(size_new.rows, vector<uint8_t>(size_new.columns, 0));

	for (int r = 0; r < size_new.rows; r++)
	{
		for (int c = 0; c < size_new.columns; c++)
		{
			double r_mapping = (double)r / (size_new.rows - 1) * (size_old.rows - 1);
			double c_mapping = (double)c / (size_new.columns - 1) * (size_old.columns - 1);
			img_new[r][c] = Image::img[(int)round(r_mapping)][(int)round(c_mapping)];
		}
	}

	Image::size = size_new;
	Image::img = img_new;
}

void Image::bicubic(ImageSize size_new)
{
	int pad = 2;
	vector<vector<uint8_t>> img_pad = Image::padding(pad);

	ImageSize size_old(Image::size);
	Image::size = size_new;
	Image::img = vector<vector<uint8_t>>(size_new.rows, vector<uint8_t>(size_new.columns, 0));

	for (int r = 0; r < size_new.rows; r++)
	{
		for (int c = 0; c < size_new.columns; c++)
		{
			double r_mapping = (double)r / (size_new.rows - 1) * (size_old.rows - 1) + pad;
			double c_mapping = (double)c / (size_new.columns - 1) * (size_old.columns - 1) + pad;
			double r1 = floor(r_mapping);
			double c1 = floor(c_mapping);

			double sum = 0;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					sum += img_pad[(int)r1 - 1 + i][(int)c1 - 1 + j]
						* bicubic_param(r_mapping - (r1 - 1 + i), -0.5)
						* bicubic_param(c_mapping - (c1 - 1 + j), -0.5);
				}
			}

			Image::img[r][c] = (uint8_t)round(sum);
		}
	}
}

void Image::threshold(uint8_t th)
{
	for (auto& i : Image::img)
		for (auto& j : i)
			j = (j < th) ? 0 : 255;
}

void Image::replaceBitPlane(Image& img_in, int bit)
{
	if (Image::size == img_in.size)
	{
		for (int r = 0; r < Image::size.rows; r++)
			for (int c = 0; c < Image::size.columns; c++)
			{
				bool binary = (img_in.img[r][c] == 255) ? 1 : 0;
				BitPlane bitplane;
				bitplane.pixel = Image::img[r][c];
				bitplane.b0 = (bit == 0) ? binary : bitplane.b0;
				bitplane.b1 = (bit == 1) ? binary : bitplane.b1;
				bitplane.b2 = (bit == 2) ? binary : bitplane.b2;
				bitplane.b3 = (bit == 3) ? binary : bitplane.b3;
				bitplane.b4 = (bit == 4) ? binary : bitplane.b4;
				bitplane.b5 = (bit == 5) ? binary : bitplane.b5;
				bitplane.b6 = (bit == 6) ? binary : bitplane.b6;
				bitplane.b7 = (bit == 7) ? binary : bitplane.b7;
				Image::img[r][c] = bitplane.pixel;
			}
	}
	else
	{
		cout << "Image::size != img_in.size" << endl;
	}
}

void Image::extractBitPlane(int bit)
{
	vector<vector<uint8_t>> img_temp(Image::size.rows, vector<uint8_t>(Image::size.columns, 0));

	for (int r = 0; r < Image::size.rows; r++)
		for (int c = 0; c < Image::size.columns; c++)
		{
			BitPlane bitplane;
			bitplane.pixel = Image::img[r][c];
			img_temp[r][c] = (bit == 0) ? (bitplane.b0 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 1) ? (bitplane.b1 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 2) ? (bitplane.b2 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 3) ? (bitplane.b3 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 4) ? (bitplane.b4 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 5) ? (bitplane.b5 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 6) ? (bitplane.b6 * 255) : img_temp[r][c];
			img_temp[r][c] = (bit == 7) ? (bitplane.b7 * 255) : img_temp[r][c];
		}

	Image::img = img_temp;
}

void Image::logTrans()
{
	const double c = 255 / log(1 + 255);
	for (auto& i : Image::img)
		for (auto& j : i)
		{
			j = (uint8_t)round(c * log(1 + j));
		}
}

void Image::invLogTrans()
{
	const double c = 255 / log(1 + 255);
	for (auto& i : Image::img)
		for (auto& j : i)
		{
			j = (uint8_t)round(exp(j / c) - 1);
		}
}

void Image::gammaTrans(double gamma)
{
	const double c = 255 / pow(255, gamma);
	for (auto& i : Image::img)
		for (auto& j : i)
		{
			j = (uint8_t)round(c * pow(j, gamma));
		}
}

void Image::negative()
{
	for (auto& i : Image::img)
		for (auto& j : i)
		{
			j = 255 - j;
		}
}

void Image::histEqualize()
{
	vector<int> hist(256, 0);
	for (auto& i : Image::img)
		for (auto& j : i)
			hist[j] += 1;
	double size = (double)Image::size.size();

	vector<uint8_t> cdf(256, 0);
	double sum = 0;
	for (int i = 0; i < 256; i++)
	{
		sum += hist[i] / size;
		cdf[i] = (uint8_t)round(sum * 255);
	}

	for (auto& i : Image::img)
		for (auto& j : i)
			j = cdf[j];
}

Image::Image()
{
	Image::size = ImageSize(0, 0);
}

Image::Image(const Image& source)
{
	Image::size = source.size;
	Image::img = source.img;
}

Image::Image(ImageSize size, uint8_t value)
{
	Image::size = size;
	Image::img = vector<vector<uint8_t>>(size.rows, vector<uint8_t>(size.columns, value));
}

Image::Image(vector<vector<uint8_t>>& img)
{
	Image::size.rows = (int)img.size();
	Image::size.columns = (int)img[0].size();
	Image::img = img;
}

Image Image::operator=(const Image& other)
{
	Image::size = other.size;
	Image::img = other.img;
	return *this;
}

vector<vector<uint8_t>> Image::padding(int pad)
{
	vector<vector<uint8_t>> img_pad(size.rows + 2 * pad, vector<uint8_t>(size.columns + 2 * pad, 0));

	for (int r = 0; r < size.rows; r++)
		for (int c = 0; c < size.columns; c++)
			img_pad[r + pad][c + pad] = Image::img[r][c];

	for (int r = 0; r < size.rows + pad * 2; r++)
	{
		for (int c = 0; c < size.columns + pad * 2; c++)
		{
			if (r < pad && c < pad)
				img_pad[r][c] = img_pad[2 * pad - 1 - r][2 * pad - 1 - c];
			else if (r < pad && pad <= c && c < size.columns + pad)
				img_pad[r][c] = img_pad[2 * pad - 1 - r][c];
			else if (r < pad && c >= size.columns + pad)
				img_pad[r][c] = img_pad[2 * pad - 1 - r][2 * (size.columns + pad) - 1 - c];
			else if (pad <= r && r < size.rows + pad && c < pad)
				img_pad[r][c] = img_pad[r][2 * pad - 1 - c];
			else if (pad <= r && r < size.rows + pad && c >= size.columns + pad)
				img_pad[r][c] = img_pad[r][2 * (size.columns + pad) - 1 - c];
			else if (r >= size.rows + pad && c < pad)
				img_pad[r][c] = img_pad[2 * (size.rows + pad) - 1 - r][2 * pad - 1 - c];
			else if (r >= size.rows + pad && pad <= c && c < size.columns + pad)
				img_pad[r][c] = img_pad[2 * (size.rows + pad) - 1 - r][c];
			else if (r >= size.rows + pad && c >= size.columns + pad)
				img_pad[r][c] = img_pad[2 * (size.rows + pad) - 1 - r][2 * (size.columns + pad) - 1 - c];
		}
	}

	return img_pad;
}

double Image::bicubic_param(double x, double a)
{
	x = abs(x);
	if (x <= 1)
		return (a + 2) * x * x * x - (a + 3) * x * x + 1;
	else if (1 < x && x < 2)
		return a * x * x * x - 5 * a * x * x + 8 * a * x - 4 * a;
	else
		return 0;
}
