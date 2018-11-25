#include "CapturePlayer.h"
#include <string>

int CapturePlayer::pictureID = 0;

CapturePlayer& operator>> (CapturePlayer& in, cv::Mat& copy)
{
	in.inputCapture >> copy;
	return in;
}

CapturePlayer::CapturePlayer(int cameraID):
	inputCapture()
{
	inputCapture.open(cameraID);
}


CapturePlayer::~CapturePlayer()
{

}


void CapturePlayer::saveShot()
{
	cv::Mat temp;
	if(inputCapture.isOpened()) {
		inputCapture >> temp;
		cv::imwrite(std::to_string(pictureID++) + ".png", temp);
	}
}