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
	//0 mean right 1 mean left
	int offsetForward;
	int offsetLeft;
	int offsetRight;
	int offsetBack;
public:
	bool direction;
	double pForward;
	double pLeft;
	double pRight;
	double pBack;
	Point nextPoint;
	int trajectoryMemorySize;
	static int howMany();
	Scalar drawingColor;
	Point getCenterPoint();
	Rect getBoundingRectangle();
	void setCenterPoint(Point p);
	void setBoundingRectangle(Rect rect);
	vector<Point> trajectory;
	bool Synchronized;
	bool isActive;
	Vehicle(void);
	Vehicle(Point CentralPoint, Rect BoundingRectangle, int trajectoryMemory);
	~Vehicle(void);
};

