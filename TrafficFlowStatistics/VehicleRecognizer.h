#pragma once
#include "Vehicle.h"
#include "cv.h"
#include "highgui.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/background_segm.hpp>

using namespace cv;

class VehicleRecognizer
{
private:
	//FileName
	string SourceFileName;
	//Current frame
	Mat frame;
	//fg mask generated by MOG method
	Mat fgMaskMOG;
	//Contours frame
	Mat ContoursFrame;
	//MOG Background substractor
	//Ptr<BackgroundSubtractor> pMOG;
	BackgroundSubtractorMOG2 MOG2;
	BackgroundSubtractorMOG MOG;
	VideoCapture capture;
	//String stream for frame number
	std::stringstream ss;
	//Use MOG2 substraction method flag
	bool mbUseMOG2;
	//threshold minimum value
	int thresholdMin;
	//Erosion operation
	int erosion_elem;
	int erosion_size;
	void Erosion();
	//Dilation operation
	int dilation_elem;
	int dilation_size;
	void Dilation();
	//Blur size
	Size blur_size;
	//Max erosion/dilation size
	int static const max_kernel_size = 21;
	//Contours vector
	vector<vector<Point>> contours;
	//Hierarchy vector
	vector<Vec4i> hierarchy;
	//Moments vector
	vector<Moments> mu;
	//Mass centers
	vector<Point2f> mc;
	bool doDrawing;
	//Get moments of the objects
	void GetMoments();
	//Approximate a polygon with another polygon
	vector<vector<Point>> contours_poly;
	//Bounding Rectangle
	vector<Rect> boundRect;
	//Minimum contour area
	int minContourArea;
	//Vector of trajectory
	vector<Point> trajectory;
	//Previous contours count
	int prevContoursCount;
	void TrackVehicles();
	//vector of vehicles
	vector<Vehicle> vehicles;
	bool PointInsideRectange(Point p, Rect rect);
public:

		//fg mask generated by MOG method
	Mat erosionMask;
	Mat dilationMask;

	bool ProcessNextFrame();
	//Returns the next frame
	Mat getNextFrameMat();
	//Returns binary frame
	Mat getNextFrameBinary();
	//Returns frame with contours
	Mat getNextContoursFrame();
	//Gets source filename.
	string getSourceVideoFileName();
	//Sets source filename.
	void setSourceVideoFileName(string SourceVideoFileName);
	//Sets threshold minimum
	void setThresholdMinimum(int value);
	//Sets blur size
	void setBlurSize(Size sz);
	//Sets erosion size
	void setErosionSize(int size);
	//Sets dilation size
	void setDilationSize(int size);
	//Enable/disable drawing mode
	void setDrawingFlag(bool doDrawing);
	//Constructor
	VehicleRecognizer(string SourceVideoFileName, 
		bool UseMOG2Substraction = false, 
		int ThresholdMinimum = 220, 
		int erosion_elem = 0, 
		int dilation_elem = 0,
		int minContoursArea = 2000
		);
	//Default constuctor
	VehicleRecognizer(void);
	//Default destructor
	~VehicleRecognizer(void);
};

