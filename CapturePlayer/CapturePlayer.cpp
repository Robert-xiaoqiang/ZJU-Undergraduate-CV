#include "CapturePlayer.h"
#include <string>
#ifndef CAPTURE_PLAYER_H_
#define CAPTURE_PLAYER_H_

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

class CapturePlayer;

CapturePlayer& operator>> (CapturePlayer& in, cv::Mat&);

class CapturePlayer {
public:
	CapturePlayer(int cameraID);
	virtual ~CapturePlayer();

	void saveShot();
private:
	friend CapturePlayer& operator>> (CapturePlayer& in, cv::Mat&);
	cv::VideoCapture inputCapture;
	static int pictureID;
};

#endif

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
		cv::Mat image_gray;
		cv::cvtColor(temp, image_gray, CV_BGR2GRAY);
		cv::imwrite(std::to_string(pictureID++) + ".png", image_gray);
	}
}