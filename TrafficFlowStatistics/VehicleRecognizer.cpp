#include "VehicleRecognizer.h"

bool VehicleRecognizer::ProcessNextFrame()
{
	if(!capture.read(frame))
		return false;
	//Update background model
	//pMOG->operator()(frame, fgMaskMOG);
	if (this->mbUseMOG2)
		MOG2(frame, fgMaskMOG);
	else
		MOG(frame, fgMaskMOG);
	//Get the frame number and write it on the current frame
	rectangle(frame, Point(10, 2), Point(100, 20), Scalar(255,255,255), -1);
	ss << capture.get(CV_CAP_PROP_POS_FRAMES);
	string frameNumberString = ss.str();
	putText(frame, frameNumberString.c_str(), Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
}

Mat VehicleRecognizer::getNextFrameMat()
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

VehicleRecognizer::VehicleRecognizer (string SourceVideoFileName, bool UseMOG2Substraction)
{
	this->mbUseMOG2 = UseMOG2Substraction;
	this->SourceFileName = SourceVideoFileName;
	//if (this->mbUseMOG2)
	//	pMOG = new BackgroundSubtractorMOG2();
	//else
	//	pMOG = new BackgroundSubtractorMOG();
	
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::VehicleRecognizer(void)
{
	this->mbUseMOG2 = false;
	this->SourceFileName = nullptr;
	//pMOG = new BackgroundSubtractorMOG();
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}
VehicleRecognizer::~VehicleRecognizer(void)
{
	capture.release();
}
