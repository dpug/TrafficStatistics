#include <iostream>
#include "VehicleRecognizer.h"
using namespace std;

int main(int argc, char* argv[])
{
	int keyboard = 0;
	namedWindow("Frame");
	VehicleRecognizer VehicleRec("source.mp4", true);
	while((char)keyboard != 'q' && (char)keyboard != 27)
	{
		if (!VehicleRec.ProcessNextFrame())
			break;
		imshow("Frame", VehicleRec.getNextFrameMat());
		keyboard = waitKey(30);
	}

	system("pause");
	cv::destroyAllWindows();
	return EXIT_SUCCESS;
}