#include "Calibrater.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>

cv::Size Calibrater::boardInnerSize = { 9, 7 };
cv::Size Calibrater::squareSize = { 20, 20 };

Calibrater::Calibrater(const std::array<std::string, Calibrater::maxImageSize>& imageFileList, const std::string& winname):
	all2dCornerPoints(),
	all3dCornerPoints(),
	imageSize(0, 0)
{
	openFile(imageFileList, winname);
	build3d();
}

Calibrater::~Calibrater()
{

}

void Calibrater::openFile(const std::array<std::string, Calibrater::maxImageSize>& imageFileList, const std::string& winname)
{
	for(int i = 0; i < imageFileList.size(); i++) {
		cv::Mat curImage = cv::imread(imageFileList[i]);
		if(!imageSize.width && !imageSize.height) imageSize = { curImage.cols, curImage.rows };
		srcImage.push_back(std::make_tuple(imageFileList[i], curImage));
		std::vector<cv::Point2f> curPoints;
		// corner point finding
		cv::findChessboardCorners(curImage, Calibrater::boardInnerSize, curPoints);
		cv::Mat tempGrayImage;
		cv::cvtColor(curImage, tempGrayImage, CV_RGB2GRAY);
		// subpixel corner point
		// by 2-value image
		// input, inout, size
		//cv::find4QuadCornerSubpix(tempGrayImage, curPoints, cv::Size(11, 11));
		cv::cornerSubPix(tempGrayImage, curPoints, cv::Size(Calibrater::boardInnerSize.width, Calibrater::boardInnerSize.height), cv::Size(-1, -1), cv::TermCriteria());
		all2dCornerPoints.push_back(curPoints);
		// show corner points
		cv::drawChessboardCorners(tempGrayImage, Calibrater::boardInnerSize, curPoints, true);
		cv::imshow(winname, tempGrayImage);
		cv::waitKey(1000);
	}
}

void Calibrater::build3d()
{
	assert(all2dCornerPoints.size() > 0);
	all3dCornerPoints.clear();
	std::vector<cv::Point3f> tempBuffer;
	for(int i = 0; i < Calibrater::boardInnerSize.height; i++)
		for(int j = 0; j < Calibrater::boardInnerSize.width; j++) {
			cv::Point3f cur = { static_cast<float>(i * Calibrater::squareSize.height),
				static_cast<float>(j * Calibrater::squareSize.width),
				0.0f };
			tempBuffer.insert(tempBuffer.end(), cur);
		}
	all3dCornerPoints.insert(all3dCornerPoints.end(), all2dCornerPoints.size(), tempBuffer);
}

void Calibrater::workAndSave(const std::string& fileName, const std::string& winname) const
{
	cv::Mat innerMatrix(3, 3, CV_32FC1, cv::Scalar::all(0.0));
	cv::Mat distortionCoe(1, 5, CV_32FC1, cv::Scalar::all(0.0));
	// k1, k2, p1, p2, k3 ... K6
	std::vector<cv::Mat> rotation, translation;

	// text mode
	std::ofstream out(fileName.c_str());
	// calibration action
	/*
	* CV_CALIB_USE_INTRINSIC_GUESS u0 v0 fx fy
	* CV_CALIB_FIX_PRINCIPAL_POINT
	* CV_CALIB_USE_INTRINSIC_GUESS u0 v0, INPUT
	* CV_CALIB_FIX_ASPECT_RATIO fx/fy fixed(set GUESS)
	* CV_CALIB_ZERO_TANGENT_DIST p1 p2
	* CV_CALIB_FIX_K# K[1-6]
	* CV_CALIB_RATIONAL_MODEL K4, K5, K6
	*/	
	cv::calibrateCamera(all3dCornerPoints,
						all2dCornerPoints,
						imageSize,
						innerMatrix,
						distortionCoe,
						rotation,
						translation);
	std::vector<double> errs;
	for(int i = 0; i < all2dCornerPoints.size(); i++) {
		out << std::setw(20) << std::setfill('-') << "" << std::endl;
		out << std::to_string(i) + "# picture's calibration result";
		out << "\ninnerMatrix:\n" << innerMatrix
			<< "\ndistortCoe:\n" << distortionCoe
			<< "\nrotation:\n" << rotation[i]
			<< "\ntranslation:\n" << translation[i] << std::endl;
		std::vector<cv::Point2f> new2dCornerPoints;
		cv::projectPoints(all3dCornerPoints[i], rotation[i], translation[i], innerMatrix, distortionCoe, new2dCornerPoints);
		cv::Mat m1(1, new2dCornerPoints.size(), CV_32FC2), m2(1, new2dCornerPoints.size(), CV_32FC2);
		for(int j = 0; j < new2dCornerPoints.size(); j++) {
			m1.at<cv::Vec2f>(0, j) = { all2dCornerPoints[i][j].x, all2dCornerPoints[i][j].y };
			m2.at<cv::Vec2f>(0, j) = { new2dCornerPoints[j].x, new2dCornerPoints[j].y };
		}
		//errs.push_back(cv::norm(m1, m2, cv::NORM_L2) / new2dCornerPoints.size());
		out << "error: " << cv::norm(m1, m2, cv::NORM_L2) / new2dCornerPoints.size() << std::endl;
	}
	cv::Mat mapx(imageSize, CV_32FC1);
	cv::Mat mapy(imageSize, CV_32FC1);
	// anti-distortion
	cv::initUndistortRectifyMap(innerMatrix, 
								distortionCoe, 
								cv::Mat::eye(3, 3, CV_32F), 
								innerMatrix, 
								imageSize, 
								CV_32FC1, 
								mapx, 
								mapy);
	// remap
	/*
	 * INTER_NEAREST - a nearest - neighbor interpolation
	 * INTER_LINEAR - a bilinear interpolation(used by default)
	 * INTER_AREA - resampling using pixel area relation.It may be a preferred method for image decimation
	 * INTER_CUBIC - a bicubic interpolation over 4x4 pixel neighborhood
	 * INTER_LANCZOS4 - a Lanczos interpolation over 8x8 pixel neighborhood
	 */
	for(int i = 0; i < srcImage.size(); i++) {
		std::string dstImageName = "after_" + std::get<0>(srcImage[i]);
		cv::Mat before = std::get<1>(srcImage[i]);
		cv::Mat after, dst;
		cv::remap(before, after, mapx, mapy, cv::INTER_LINEAR);
		dst.create(before.rows, before.cols + 10 + after.cols, before.type());
		before.copyTo(dst(cv::Rect(0, 0, before.cols, before.rows)));
		after.copyTo(dst(cv::Rect(before.cols + 10, 0, after.cols, after.rows)));
		cv::putText(dst, "before", cv::Point2i(0, dst.rows - 10), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.0, cv::Scalar(255.0, 0.0, 0.0), 2, false);
		cv::putText(dst, " after", cv::Point2i(before.cols, dst.rows - 10), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 1.0, cv::Scalar(255.0, 0.0, 0.0), 2, false);
		cv::imshow(winname, dst);
		cv::waitKey(2000);
		cv::imwrite(dstImageName, dst);
	}
}
