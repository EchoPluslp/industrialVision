#pragma once
#ifndef ALGORITHMLIBRARY_H
#define ALGORITHMLIBRARY_H

#include <QImage>
#include <vector>
#include <math.h>
#include<opencv2\opencv.hpp>
using namespace cv;
class  algorithmLibrary {

};
namespace Tools {
	QImage PowerLevelTransformation(const QImage& origin, const QImage& model, double gamma);

}
#endif 