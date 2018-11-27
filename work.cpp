#define _CRT_SECURE_NO_WARNINGS
#include "CapturePlayer/CapturePlayer.h"
#include "Calibrater/Calibrater.h"
#include <iostream>

int main(int argc, char *argv[])
{
	cv::namedWindow("A Window");
	std::array<std::string, Calibrater::maxImageSize> list = {
		"image/0.png",
		"image/1.png",
		"image/2.png",
		"image/3.png",

		"image/4.png",
		"image/5.png",
		"image/6.png",
		"image/7.png",

		"image/8.png",
		"image/9.png"
	};
	Calibrater cl(list, "A Window");
	cl.workAndSave("calibration.result");
	cl.showDifference("A Window");
	//if(argc < 2) {
	//	std::cout << "work captrue or\nwork calibrate" << std::endl;
	//}
	//if(argv[1] == std::string("capture")) {
	//	CapturePlayer cp(0);
	//	while(true) {
	//		cv::Mat temp;
	//		cp >> temp;
	//		cv::imshow("A Window", temp);
	//		char c = cv::waitKey(10);
	//		if(c == 27) {
	//			break;
	//		} else if(c == 'c') {
	//			cp.saveShot();
	//		}
	//	}
	//} else if(argv[1] == std::string("calibrate")) {
	//	std::array<std::string, Calibrater::maxImageSize> list = {
	//		"image/0.png",
	//		"image/1.png",
	//		"image/2.png",
	//		"image/3.png",

	//		"image/4.png",
	//		"image/5.png",
	//		"image/6.png",
	//		"image/7.png",

	//		"image/8.png",
	//		"image/9.png"
	//	};
	//	Calibrater cl(list, "A Window");
	//	cl.workAndSave("calibration.result");
	//}
	return 0;
}