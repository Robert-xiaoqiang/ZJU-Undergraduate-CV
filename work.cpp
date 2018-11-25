#include "CapturePlayer/CapturePlayer.h"
#include "Calibrater/Calibrater.h"
#include <iostream>

int main(int argc, char *argv[])
{
	cv::namedWindow("A Window");
	if(argc < 2) {
		std::cout << "work captrue or\nwork calibrate" << std::endl;
	}
	if(argv[1] == std::string("capture")) {
		CapturePlayer cp(0);
		while(true) {
			cv::Mat temp;
			cp >> temp;
			cv::imshow("A Window", temp);
			char c = cv::waitKey(10);
			if(c == 27) {
				break;
			} else if(c == 'c') {
				cp.saveShot();
			}
		}
	} else if(argv[1] == std::string("calibrate")) {
		std::array<std::string, Calibrater::maxImageSize> list = {
			"image2/00.jpg",
			"image2/01.jpg",
			"image2/02.jpg",
			"image2/03.jpg",

			"image2/04.jpg",
			"image2/05.jpg",
			"image2/06.jpg",
			"image2/07.jpg",

			"image2/08.jpg",
			"image2/09.jpg",
			/*"image/10.png",
			"image/11.png",
			"image/12.png",
			"image/13.png",

			"image/14.png",
			"image/15.png",
			"image/16.png",
			"image/17.png",

			"image/18.png"*/
		};
		Calibrater cl(list, "A Window");
		cl.workAndSave("calibration.result");
	}
	return 0;
}
