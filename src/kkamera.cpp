#include <iostream>
#include <kkamera.h>

using namespace cv;

// Adjust contrast (alpha) and brightness (beta) on an image given by its path.
// We pass in "bbeta" because we don't want to mix with std::beta, as we don't use the namespace.
// alpha in [1.0,3.0]
// beta in [0,100]
Mat adjust_contrast_and_brightness(Mat image, float alpha, float bbeta)
{
	Mat modified_image = image.clone();

	// Modify constrast and brightness pixel by pixel.
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

	return modified_image;
}

// Read in and return a Mat image of the file located at imgpath.
Mat import_image(char *imgpath) 
{
	std::cout << "Importing from " << imgpath << std::endl;

	Mat image = imread(imgpath, 1);
	if (!image.data) {
		std::cout << "No image data." << std::endl;
		return Mat{};	
	}
	Mat result = adjust_contrast_and_brightness(image, 0.5, 80);
	return result;
}

// Write a Mat image to the path defined by imgpath.
void export_image(Mat image, char *imgpath) 
{
	std::cout << "Exporting to " << imgpath << std::endl;

	if (imwrite(imgpath, image) == false) {
		std::cout << "Image could not be saved." << std::endl;
	}
	std::cout << "Image successfully saved!" << std::endl;
}