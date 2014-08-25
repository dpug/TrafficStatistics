#include "VehicleRecognizer.h"

bool VehicleRecognizer::ProcessNextFrame()
{
	if(!capture.read(frame))
		return false;
	//If this is the first frame
	if(this->isTheFirstFrame)
	{
		//Initialize borders
		this->leftBorder = frame.cols * 0.06;
		this->rightBorder = frame.cols - this->leftBorder;
		this->isTheFirstFrame = false;
	}
	//Blur image
	blur(frame, frame, blur_size);
	//Update background model
	//pMOG->operator()(frame, fgMaskMOG);
	if (this->mbUseMOG2)
		MOG2(frame, fgMaskMOG);
	else
		MOG(frame, fgMaskMOG);
	//Get rid of gray colors and convert to binary image using threshold operation
	threshold(fgMaskMOG, fgMaskMOG, this->thresholdMin, 255, THRESH_BINARY);
	//Blur image
	//Apply the erosion operation
	Erosion();
	//Apply the dilation operation
	Dilation();
	//Find contours
	findContours(fgMaskMOG, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
	//Get the moments
	GetMoments();

	//Clear all contours < than min contour area
	for(int i = 0; i < contours.size();)
	{
		if (mu[i].m00 <= this->minContourArea)
		{
			contours.erase(contours.begin() + i);
			mu.erase(mu.begin() + i);
			mc.erase(mc.begin() + i);
		}
		else
			i++;
	}

	//Get bounding rectangles
	contours_poly = vector<vector<Point>>(contours.size());
	boundRect = vector<Rect>(contours.size());
	for(int i = 0; i < contours.size(); i++)
	{
		//Make approximation
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
	}

	TrackVehicles();

	//Draw contours and mass centers if needed
	if (doDrawing)
	{
		for( int i = 0; i < vehicles.size(); i++ )
		 {
			 if (!vehicles[i].isActive) continue;
			 //if (mu[i].m00 > this->minContourArea)
			 //{
				//drawContours( frame, contours, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point() );
				//Draw bounding rectangle
				//rectangle( frame, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0 );
			 rectangle(frame, vehicles[i].getBoundingRectangle().tl(), vehicles[i].getBoundingRectangle().br(), Scalar(0, 0, 255), 2, 8, 0);

			/*if (mc[i] != Point2f(0,0))
				circle(frame, mc[i], 20, Scalar(0, 255, 0), -1, 4, 0);*/
			 if (vehicles[i].getCenterPoint() != Point(0,0))
				 circle(frame, vehicles[i].getCenterPoint(), 20, Scalar(0, 255, 0), -1, 4, 0);
			 //}
		 }
		//Draw all trajectories
		for (int i = 0; i < vehicles.size(); i++)
		{
			//Scalar sc = Scalar(rand() % 255, rand() % 255, rand() % 255);
			if (!vehicles[i].isActive) continue;
			if (vehicles[i].trajectory.size() > 2)
			{
				for(int j = 0; j < vehicles[i].trajectory.size() - 1; ++j)
				{
					line(frame, vehicles[i].trajectory[j], vehicles[i].trajectory[j+1], vehicles[i].drawingColor, 3);
				}
			}
			//Draw next point
			if(vehicles[i].nextPoint != Point(0,0))
			{
				circle(frame, vehicles[i].nextPoint, 5, vehicles[i].drawingColor, -1, 4, 0);
			}
			//Draw direction
			string direction = "Direction: Right";
			if (vehicles[i].direction)
				direction = "Direction: Left";
			putText(frame, direction, Point(vehicles[i].getCenterPoint().x + 50, vehicles[i].getCenterPoint().y - 20), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),2,8);
			putText(frame, "P FORWARD: " + stringify(vehicles[i].pForward), Point(vehicles[i].getCenterPoint().x + 50, vehicles[i].getCenterPoint().y - 35), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),2,8);
			putText(frame, "P BACK: " + stringify(vehicles[i].pBack), Point(vehicles[i].getCenterPoint().x + 50, vehicles[i].getCenterPoint().y - 50), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),2,8);
			putText(frame, "P LEFT: " + stringify(vehicles[i].pLeft), Point(vehicles[i].getCenterPoint().x + 50, vehicles[i].getCenterPoint().y - 65), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),2,8);
			putText(frame, "P RIGHT: " + stringify(vehicles[i].pRight), Point(vehicles[i].getCenterPoint().x + 50, vehicles[i].getCenterPoint().y - 80), FONT_HERSHEY_PLAIN, 1, Scalar(255,255,255),2,8);
		}

		//Put text
		putText(frame, "Vehicles: " + std::to_string((_ULonglong)Vehicle::howMany()), Point(20,20), FONT_HERSHEY_PLAIN, 1,  Scalar(0,0,255), 2, 8);
	}
	return true;
}


std::string VehicleRecognizer::stringify(double x)
 {
   std::ostringstream o;
   if (!(o << x))
	   throw new std::exception("stringify(double)");
   return o.str();
 }

Mat VehicleRecognizer::getNextFrameMat()
{
	return frame;
}

Mat VehicleRecognizer::getNextFrameBinary()
{
	return fgMaskMOG;
}

//Gets source filename.
string VehicleRecognizer::getSourceVideoFileName()
{
	return SourceFileName;
}
//Sets source filename.
void VehicleRecognizer::setSourceVideoFileName(string SourceVideoFileName)
{
	SourceFileName = SourceVideoFileName;
}

//Sets threshold minimum
void VehicleRecognizer::setThresholdMinimum(int value)
{
	if (value > 0 && value <= 255)
		this->thresholdMin = value;
	else throw std::exception("Threshold value must be in the interval (0,255]");
}

VehicleRecognizer::VehicleRecognizer (string SourceVideoFileName, bool UseMOG2Substraction, int ThresholdMinimum, int erosion_elem, int dilation_elem, int minContoursArea)
{
	this->leftBorder = 0;
	this->rightBorder = 0;
	this->isTheFirstFrame = true;
	this->prevContoursCount = 0;
	this->doDrawing = true;
	this->erosion_elem = erosion_elem;
	this->erosion_size = 0;
	this->dilation_elem = dilation_elem;
	this->dilation_size = 0;
	this->minContourArea = minContoursArea;
	this->thresholdMin = ThresholdMinimum;
	this->mbUseMOG2 = UseMOG2Substraction;
	this->SourceFileName = SourceVideoFileName;
	this->blur_size = Size(3,3);
	this->vehicles = vector<Vehicle>();
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::VehicleRecognizer(void)
{
	this->isTheFirstFrame = true;
	this->leftBorder = 0;
	this->rightBorder = 0;
	this->prevContoursCount = 0;
	this->doDrawing = true;
	erosion_elem = 0;
	erosion_size = 0;
	dilation_elem = 0;
	dilation_size = 0;
	this->thresholdMin = 220;
	this->minContourArea = 2000;
	this->mbUseMOG2 = false;
	this->SourceFileName = nullptr;
	this->blur_size = Size(3,3);
	this->vehicles = vector<Vehicle>();
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::~VehicleRecognizer(void)
{
	capture.release();
}

/**  @function Erosion */
void VehicleRecognizer::Erosion()
{
	int erosion_type;
	if( erosion_elem == 0 ){ erosion_type = MORPH_RECT; }
		else if( erosion_elem == 1 ){ erosion_type = MORPH_CROSS; }
		else if( erosion_elem == 2) { erosion_type = MORPH_ELLIPSE; }
		else erosion_type = MORPH_RECT;
	//Create erosion element
	Mat element = getStructuringElement( erosion_type,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
	//Apply the erosion operation
	erode(fgMaskMOG, fgMaskMOG, element);
}
/** @function Dilation */
void VehicleRecognizer::Dilation()
{
	int dilation_type;
	if( dilation_elem == 0 ){ dilation_type = MORPH_RECT; }
		else if( dilation_elem == 1 ){ dilation_type = MORPH_CROSS; }
		else if( dilation_elem == 2) { dilation_type = MORPH_ELLIPSE; }
		else dilation_type = MORPH_RECT;
	//Create dilation element
	Mat element = getStructuringElement( dilation_type,
                                    Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                    Point( dilation_size, dilation_size ) );
	//Apply the dilation operation
	dilate(fgMaskMOG, fgMaskMOG, element);
}

//Sets blur size
void VehicleRecognizer::setBlurSize(Size sz)
{
	blur_size = sz;
}


//Sets erosion size
void VehicleRecognizer::setErosionSize(int size)
{
	if(size <= max_kernel_size)
		this->erosion_size = size;
	else throw std::exception("Invalid Erosion size");
}

//Sets dilation size
void VehicleRecognizer::setDilationSize(int size)
{
	if(size <= max_kernel_size)
		this->dilation_size = size;
	else throw std::exception("Invelid dilation size");
}

//Gets next contours frame
Mat VehicleRecognizer::getNextContoursFrame()
{
	return this->ContoursFrame;
}

void VehicleRecognizer::setDrawingFlag(bool doDrawing)
{
	this->doDrawing = doDrawing;
}

//Gets the moments of contours.
void VehicleRecognizer::GetMoments()
{
	//Allocate vector size for all moments
	mu = vector<Moments>(contours.size());
	for(int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], true);
	}
	//Get the mass centers
	mc = vector<Point2f>(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		if (mu[i].m00 > this->minContourArea)
			mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
		else
			mc[i] = Point2f(0, 0);
	}

}

//Tracks vehicles
void VehicleRecognizer::TrackVehicles()
{
	//Loop thru all vehicles and make them unsynchronized
	for(int i = 0; i < vehicles.size(); i++)
		vehicles[i].Synchronized = false;

	//Loop thru all contours
	for(int i = 0; i < contours.size(); i++)
	{
		bool vehicleFound = false;
		//Finding vehicle
		for(int j = 0; j < vehicles.size(); j++)
		{
			//IF CENTER OF CONTOUR IS INSIDE VEHICLE RECTANGLE
			if (PointInsideRectange(mc[i], vehicles[j].getBoundingRectangle()))
			{
				//FLAG VEHICLE USED
				vehicles[j].Synchronized = true;
				//UPDATE VEHICLE DATA
				vehicles[j].setCenterPoint(mc[i]);
				vehicles[j].setBoundingRectangle(boundRect[i]);
				vehicles[j].isActive = true;
				//FLAG IS FOUND
				vehicleFound = true;
				break;
			}
		}
		if (!vehicleFound)
		{
			//Create new vehicle
			Vehicle v = Vehicle(mc[i], boundRect[i], 10);
			v.Synchronized = true;
			vehicles.push_back(v);
		}
	}
	//LOOP THRU ALL VEHICLES
	for(int i = 0; i < vehicles.size();)
	{
		//DELETE UNUSED
		if (!vehicles[i].Synchronized)
		{
			if (vehicles[i].getCenterPoint().x > leftBorder && vehicles[i].getCenterPoint().x < rightBorder )
			{
				//Make this vehicle inactive
				vehicles[i].isActive = false;
				//vehicles[i].setCenterPoint(vehicles[i].nextPoint);
				//Move to inactive
				inactive_vehicles.push_back(vehicles[i]);
				vehicles.erase(vehicles.begin() + i);
				i++;
			}
			else
			{
				//Delete only active vehicles
				if (vehicles[i].isActive)
					vehicles.erase(vehicles.begin() + i);
				else
					i++;
			}
		}
		else i++;
	}
}

//Checks if point is inside the rectangle
bool VehicleRecognizer::PointInsideRectange(Point p, Rect rect)
{
	return p.inside(rect);
}

