#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>

#include "../include/funcs.h"

using namespace cv;

// Adjust contrast (alpha) and brightness (beta) on an image given by its path.
// We pass in "bbeta" because we don't want to mix with std::beta, as we don't use the namespace.
// alpha in [1.0,3.0]
// beta in [0,100]
void adjust_contrast_and_brightness(char *imgpath, float alpha, float bbeta) {
	Mat image = imread(imgpath, 1);
	if (!image.data) return;
	// Mat new_image = Mat::zeros(image.size(), image.type());
	Mat modified_image = image.clone();

	// Modify constrast and brightness pixel by pixel basically.
	// We scan across the x-y, using c to iterate through the number of channels 
	// (i.e RGB has three channels).
	// Saturate cast that son of a gun!
	// Using g(x,y)[c] = alpha*g(x,y)[c] + beta
	for (int y = 0; y < image.rows; y++) {
		for (int x = 0; x < image.cols; x++) {
			for (int c = 0; c < image.channels(); c++) {
				modified_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(alpha*image.at<Vec3b>(y,x)[c] + bbeta);
			}
		}
	}
	imshow("Original", image);
	imshow("Modified", modified_image);
	waitKey(0);

	return;
}