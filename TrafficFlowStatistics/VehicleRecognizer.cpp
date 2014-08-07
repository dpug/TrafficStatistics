#include "VehicleRecognizer.h"

bool VehicleRecognizer::ProcessNextFrame()
{
	if(!capture.read(frame))
		return false;
	//Update background model
	pMOG->operator()(frame, fgMaskMOG);
	pMOG2->operator()(frame, fgMaskMOG2);
	//Get the frame number and write it on the current frame
	rectangle(frame, Point(10, 2), Point(100, 20), Scalar(255,255,255), -1);
	ss << capture.get(CV_CAP_PROP_POS_FRAMES);
	string frameNumberString = ss.str();
	putText(frame, frameNumberString.c_str(), Point(15, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 0, 0));
}

Mat VehicleRecognizer::getNextFrameMat()
{
	return fgMaskMOG2;
}

//Gets source filename.
char* VehicleRecognizer::getSourceVideoFileName()
{
	return this->SourceFileName;
}
//Sets source filename.
void VehicleRecognizer::setSourceVideoFileName(char *SourceVideoFileName)
{
	this->SourceFileName = SourceFileName;
}

VehicleRecognizer::VehicleRecognizer (char *SourceVideoFileName)
{
	this->SourceFileName = SourceVideoFileName;
	pMOG = new BackgroundSubtractorMOG();
	pMOG2 = new BackgroundSubtractorMOG2();
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::VehicleRecognizer(void)
{
	this->SourceFileName = nullptr;
	pMOG = new BackgroundSubtractorMOG();
	pMOG2 = new BackgroundSubtractorMOG2();
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}
VehicleRecognizer::~VehicleRecognizer(void)
{
	delete(SourceFileName);
	capture.release();
}


