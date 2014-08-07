#include <iostream>
#include "VehicleRecognizer.h"
using namespace std;

int main(int argc, char* argv[])
{
	//Old method
	//IplImage *currentFrame = 0;

	//try
	//{
	//	cvNamedWindow("output");
	//	VehicleRecognizer VehicleRec("source.mp4");
	//	for(;;)
	//	{
	//		VehicleRec.ProcessNextFrame();
	//		currentFrame = VehicleRec.getNextFrame();
	//		if (!currentFrame)
	//			break;
	//		cvShowImage("output", currentFrame);
	//		cvWaitKey(24);
	//	}
	//}
	//catch(exception &exc)
	//{
	//	cout << exc.what() << endl;
	//}

	//cvReleaseImage(&currentFrame);
	namedWindow("Frame");
	//namedWindow("FG Mask MOG");
	VehicleRecognizer VehicleRec("source.mp4");
	for(;;)
	{
		if (!VehicleRec.ProcessNextFrame())
			break;
		imshow("Frame", VehicleRec.getNextFrameMat());
		cvWaitKey(20);
	}

	system("pause");
	cv::destroyAllWindows();
	return 0;
}