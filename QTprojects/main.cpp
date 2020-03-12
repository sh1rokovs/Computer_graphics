#include <QtCore/QCoreApplication>
#include <QImage>
#include <string>
#include <iostream>
#include "filters.h"

using namespace std;

int main(int argc, char* argv[])
{
	string s;
	QImage photo;
	Invert_filter* invert = new Invert_filter();
	Blur_filter* blur = new Blur_filter();

	for (int i = 0; i < argc; i++)
	{
		if (!strcmp(argv[i], "-p") && (i + 1 < argc))
			s = argv[i + 1];
	}

	photo.load(QString(s.c_str()));
	photo.save("tmp.jpg");

	QImage invertImage = invert->calculateNewImagePixMap(photo, 0);
	invertImage.save("Invert.jpg");
	QImage blurImage = blur->calculateNewImagePixMap(photo, 0);
	blurImage.save("blur.jpg");
}
