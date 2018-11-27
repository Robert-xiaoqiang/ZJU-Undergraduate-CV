#ifndef CALIBRATER_H_
#define CALIBRATER_H_

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <string>
#include <array>
#include <tuple>

class Calibrater {
public:
	const static int maxImageSize = 10;
	Calibrater(const std::array<std::string, Calibrater::maxImageSize>& imageFileList, const std::string& winname);
	void workAndSave(const std::string& outFile) const;
	void showDifference(const std::string& winname) const;
	virtual ~Calibrater();
private:
	static cv::Size boardInnerSize;
	static cv::Size squareSize;
	std::vector<std::vector<cv::Point2f>> all2dCornerPoints;
	std::vector<std::vector<cv::Point3f>> all3dCornerPoints;
	cv::Size imageSize;
	//std::array<cv::Mat, maxImageSize> srcImage;
	std::vector<std::tuple<std::string, cv::Mat>> srcImage;
	void openFile(const std::array<std::string, Calibrater::maxImageSize>& imageFileList, const std::string& winname);
	void build3d();
};

#endif
