#include "VehicleRecognizer.h"

bool VehicleRecognizer::ProcessNextFrame()
{
	if(!capture.read(frame))
		return false;
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

	//Draw contours and mass centers if needed
	if (doDrawing)
	{
		for( int i = 0; i< contours.size(); i++ )
		 {
			 //drawContours( frame, contours, i, Scalar(255,0,0), 2, 8, hierarchy, 0, Point() );
			 if (mc[i] != Point2f(0,0))
				circle(frame, mc[i], 20, Scalar(0, 255, 0), -1, 8, 0);
		 }
	}
	return true;
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

VehicleRecognizer::VehicleRecognizer (string SourceVideoFileName, bool UseMOG2Substraction, int ThresholdMinimum, int erosion_elem, int dilation_elem)
{
	this->doDrawing = true;
	this->erosion_elem = erosion_elem;
	this->erosion_size = 0;
	this->dilation_elem = dilation_elem;
	this->dilation_size = 0;
	this->thresholdMin = ThresholdMinimum;
	this->mbUseMOG2 = UseMOG2Substraction;
	this->SourceFileName = SourceVideoFileName;
	this->blur_size = Size(3,3);
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::VehicleRecognizer(void)
{
	this->doDrawing = true;
	erosion_elem = 0;
	erosion_size = 0;
	dilation_elem = 0;
	dilation_size = 0;
	this->thresholdMin = 220;
	this->mbUseMOG2 = false;
	this->SourceFileName = nullptr;
	this->blur_size = Size(3,3);
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
		if (mu[i].m00 > 2000)
			mc[i] = Point2f(mu[i].m10/mu[i].m00, mu[i].m01/mu[i].m00);
		else
			mc[i] = Point2f(0, 0);
	}

}