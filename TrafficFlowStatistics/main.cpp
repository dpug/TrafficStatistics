#include <iostream>
#include "VehicleRecognizer.h"
using namespace std;

int main(int argc, char* argv[])
{
	int keyboard = 0;

	//Vehicle Recognition parameters
	int erosion_size = 1;
	int dilation_size = 7;

	//OpenCV windows
	//namedWindow("Frame");
	namedWindow("Binary");
	//namedWindow("Erosion");
	//namedWindow("Dilation");
	createTrackbar( "Erosion size:\n 2n +1", "Binary", 
                  &erosion_size, 21);
	createTrackbar( "Dilation size:\n 2n +1", "Binary", 
		&dilation_size, 21);


	//Vehicle Recognizer set parameters
	VehicleRecognizer VehicleRec("source.mp4", true, 250, 0, 0);
	VehicleRec.setBlurSize(Size(2,2));
	VehicleRec.setErosionSize(erosion_size);
	VehicleRec.setDilationSize(dilation_size);


	while((char)keyboard != 'q' && (char)keyboard != 27)
	{
		if (!VehicleRec.ProcessNextFrame())
			break;

		VehicleRec.setErosionSize(erosion_size);
		VehicleRec.setDilationSize(dilation_size);
		//imshow("Frame", VehicleRec.getNextFrameMat());
		imshow("Binary", VehicleRec.getNextFrameMat());
		//imshow("Erosion", VehicleRec.erosionMask);
		//imshow("Dilation", VehicleRec.dilationMask);
		keyboard = waitKey(30);
	}
	system("pause");

	cv::destroyAllWindows();
	exit(0);
}