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
	//Get rid of gray colors and convert to binary image using threshold operation
	threshold(fgMaskMOG, fgMaskMOG, 200, 255, THRESH_BINARY);
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
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}

VehicleRecognizer::VehicleRecognizer(void)
{
	this->mbUseMOG2 = false;
	this->SourceFileName = nullptr;
	capture = VideoCapture(this->SourceFileName);
	if (!capture.isOpened())
		throw std::exception("Can't open source filename!");
}
VehicleRecognizer::~VehicleRecognizer(void)
{
	capture.release();
}
