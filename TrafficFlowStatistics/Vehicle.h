#pragma once
#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;

class Vehicle
{
private:
	Point CenterPoint;
	Rect BoundingRectangle;
	static int count;
public:
	static int howMany();
	Scalar drawingColor;
	Point getCenterPoint();
	Rect getBoundingRectangle();
	void setCenterPoint(Point p);
	void setBoundingRectangle(Rect rect);
	vector<Point> trajectory;
	bool Synchronized;
	Vehicle(void);
	Vehicle(Point CentralPoint, Rect BoundingRectangle);
	~Vehicle(void);
};

