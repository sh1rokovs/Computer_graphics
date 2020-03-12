#pragma once
#include <iostream>
#include <QImage.h>

using namespace std;

template<class T>

T clamp(T v, int max, int min)
{
	if (v > max)
		return max;
	else if (v < min)
		return min;

	return v;
}

class filter
{
public:
	filter() {};
	~filter() {};

	virtual QImage calculateNewImagePixMap(const QImage& photo, int radius) = 0;
};
class Invert_filter : public filter
{
public:
	Invert_filter() {};
	~Invert_filter() {};

	QImage calculateNewImagePixMap(const QImage& photo, int radius);
};

QImage Invert_filter::calculateNewImagePixMap(const QImage& photo, int radius)
{
	QImage result_Image(photo);

	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);

			photo_color.setRgb(255 - photo_color.red(),
				255 - photo_color.green(),
				255 - photo_color.blue());
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}
class Matrix_filter : public filter
{
public:
	float* vector;
	int mRadius;
	Matrix_filter(int radius = 1) : mRadius(radius) {};
public:
	QImage calculateNewImagePixMap(const QImage& photo, int radius);
	QColor calculateNewPixelColor(QImage photo, int x, int y, int radius);
};
QImage Matrix_filter::calculateNewImagePixMap(const QImage& photo, int radius)
{
	QImage result_Image(photo);

	for (int x = 0; x < photo.width(); x++)
		for (int y = 0; y < photo.height(); y++)
		{
			QColor photo_color = photo.pixelColor(x, y);

			photo_color.setRgb(255 - photo_color.red(),
				255 - photo_color.green(),
				255 - photo_color.blue());
			result_Image.setPixelColor(x, y, photo_color);
		}
	return result_Image;
}
class Gaussian_blur_filter : public Matrix_filter
{
public:
	Gaussian_blur_filter()
	{
		createGaussianVector(3, 2);
	}
	~Gaussian_blur_filter() {};

	void createGaussianVector(int radius, int sigma);
};

QColor Matrix_filter::calculateNewPixelColor(QImage photo, int x, int y, int radius)
{
	int returnR = 0;
	int returnG = 0;
	int returnB = 0;
	int size = 2 * radius + 1;

	for (int i = -radius; i <= radius; i++)
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + j + radius;

			QColor color = photo.pixelColor(clamp<int>(x + j, photo.width() - 1, 0),
				clamp<int>(y + i, photo.height() - 1, 0));
			returnR += color.red() * vector[idx];
			returnG += color.green() * vector[idx];
			returnB += color.blue() * vector[idx];
		}
	return QColor(clamp(returnR, 255, 0),
		clamp(returnG, 255, 0),
		clamp(returnB, 255, 0));
}

class Blur_filter : public Matrix_filter
{
public:
	Blur_filter()
	{
		int size = 2 * mRadius + 1;
		vector = new float[size * size];
		for (int i = 0; i < size; i++)
			for (int j = 0; j < size; j++)
				vector[i * size + j] = 1.0f / (size * size);
	}
};
void Gaussian_blur_filter::createGaussianVector(int radius, int sigma)
{
	const unsigned int size = 2 * radius + 1;
	double norm = 0;
	vector = new float[size * size];

	for (int i = -radius; i <= radius; i++)
	{
		for (int j = -radius; j <= radius; j++)
		{
			int idx = (i + radius) * size + (j + radius);
			vector[idx] = exp(-(i * i + j * j) / (sigma * sigma));
			norm += vector[idx];
		}
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			vector[i * size + j] /= norm;
		}
	}
}