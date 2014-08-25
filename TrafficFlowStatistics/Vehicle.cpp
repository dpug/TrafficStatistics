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
	pForward = 0;
	pLeft = 0;
	pRight = 0;
	pBack = 0;
	nextPoint = Point(0,0);
	direction = false;
	isActive = true;
	offsetForward = 0;
	offsetLeft = 0;
	offsetRight = 0;
	offsetBack = 0;
	trajectoryMemorySize = 10;
}

Vehicle::Vehicle(Point CentralPoint, Rect BoundingRectangle, int trajectoryMemory)
{
	this->drawingColor = Scalar(rand() % 255, rand() % 255, rand() % 255);
	this->Synchronized = false;
	this->CenterPoint = CentralPoint;
	this->BoundingRectangle = BoundingRectangle;
	vector<Point> trajectory = vector<Point>();
	count++;
	pForward = 0;
	pLeft = 0;
	pRight = 0;
	pBack = 0;
	nextPoint = Point(0,0);
	direction = false;
	isActive = true;
	offsetForward = 0;
	offsetLeft = 0;
	offsetRight = 0;
	offsetBack = 0;
	trajectoryMemorySize = trajectoryMemory;
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
		//Calculate vehicle direction
		if (this->trajectory.size() == 1)
		{
			if (p.x - this->CenterPoint.x > 0)
				direction = false;
			else
				direction = true;
		}
		//Push back current central point to trajectory
		this->trajectory.push_back(this->CenterPoint);
		//Calculate the next point
		double dX = p.x - this->CenterPoint.x;
		double dY = p.y - this->CenterPoint.y;
		this->nextPoint = Point(p.x+dX, p.y + dY);
		//Move bounding rectangle for inactive vehicles
		if(!this->isActive)
		{
			Rect rect = Rect(this->BoundingRectangle.x + dX, this->BoundingRectangle.y + dY, this->BoundingRectangle.width, this->BoundingRectangle.height);
			this->BoundingRectangle = rect;
		}
		//flush all memory
		if (trajectory.size() % trajectoryMemorySize == 0)
		{
			offsetForward = 0;
			offsetBack = 0;
			offsetRight = 0;
			offsetLeft = 0;
		}
		//Increment offsets
		if (!direction)
		{
			if (dX > 0)
				offsetForward += abs(dX);
			else
				offsetBack += abs(dX);
			if (dY > 0)
				offsetRight += abs(dY);
			else
				offsetLeft += abs(dY);
		}
		else
		{
			if (dX < 0)
				offsetForward += abs(dX);
			else
				offsetBack += abs(dX);
			if (dY < 0)
				offsetRight += abs(dY);
			else
				offsetLeft += abs(dY);
		}
		//Calculate total offset
		double offsetTotal = offsetForward + offsetBack + offsetLeft + offsetRight;
		//Calculate probabilities
		pForward = offsetForward/offsetTotal;
		pBack = offsetBack/offsetTotal;
		pLeft = offsetLeft/offsetTotal;
		pRight = offsetRight/offsetTotal;
	//	//Calculate probabilities
	//	double totalD = abs(dX) + abs(dY);  //total changes
	//	//Horizontal probability
	//	dX = dX/totalD;
	//	//Vertical probability
	//	dY = dY/totalD;
	//	//If object goes right
	//	if (!direction)
	//	{
	//		if (dX > 0)
	//		{
	//			pForward = dX;
	//			pBack = 0.0;
	//		}
	//		else
	//		{
	//			pBack = abs(dX);
	//			pForward = 0.0;
	//		}
	//		if (dY > 0)
	//		{
	//			pRight = dY;
	//			pLeft = 0.0;
	//		}
	//		else
	//		{
	//			pLeft = abs(dY);
	//			pRight = 0.0;
	//		}
	//	}
	//	else
	//	{
	//		if (dX < 0)
	//		{
	//			pForward = abs(dX);
	//			pBack = 0.0;
	//		}
	//		else
	//		{
	//			pBack = dX;
	//			pForward = 0.0;
	//		}
	//		if (dY < 0)
	//		{
	//			pRight = abs(dY);
	//			pLeft = 0.0;
	//		}
	//		else
	//		{
	//			pLeft = dY;
	//			pRight = 0.0;
	//		}
	//	}
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
