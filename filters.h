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
private:
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

	for(int x = 0; x < photo.width(); x++)
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