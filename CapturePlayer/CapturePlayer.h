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
