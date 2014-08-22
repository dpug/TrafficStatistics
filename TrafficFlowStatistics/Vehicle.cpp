#include "Vehicle.h"

int Vehicle::count = 0;

Vehicle::Vehicle(void)
{
	this->drawingColor = Scalar(rand() % 255, rand() % 255, rand() % 255);
	this->Synchronized = false;
	CenterPoint = Point(0, 0);
	BoundingRectangle = Rect(0, 0, 0, 0);
	vector<Point> trajectory = vector<Point>();
	count++;
}

Vehicle::Vehicle(Point CentralPoint, Rect BoundingRectangle)
{
	this->drawingColor = Scalar(rand() % 255, rand() % 255, rand() % 255);
	this->Synchronized = false;
	this->CenterPoint = CentralPoint;
	this->BoundingRectangle = BoundingRectangle;
	vector<Point> trajectory = vector<Point>();
	count++;
}

Vehicle::~Vehicle(void)
{
	this->trajectory.clear();
}


Point Vehicle::getCenterPoint()
{
	return this->CenterPoint;
}

Rect Vehicle::getBoundingRectangle()
{
	return this->BoundingRectangle;
}


void Vehicle::setCenterPoint(Point p)
{
	if (this->CenterPoint != Point(0,0))
	{
		//Push back current central point to trajectory
		this->trajectory.push_back(this->CenterPoint);
	}
	this->CenterPoint = p;
}

void Vehicle::setBoundingRectangle(Rect rect)
{
	this->BoundingRectangle = rect;
}

int Vehicle::howMany()
{
	return count;
}
