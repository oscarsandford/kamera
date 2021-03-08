// Kamera (KMR) functions to do with OpenCV Mat images.
// Does a lot of the heavy lifting when it comes to loading, 
// modifying, and saving images.
#include <kutils.h>
#include <iostream>

using namespace cv;

// Read in and return a Mat image of the file located at path.
Mat KMR_import(char *path) 
{
	std::cout << "Importing from " << path << std::endl;
	Mat image = imread(path, 1);
	if (!image.data) 
	{
		std::cout << "[!] No image data." << std::endl;
		return Mat{};	
	}
	return image;
}

// Write a Mat image to the path defined by path.
void KMR_export(Mat image, char *path) 
{
	std::cout << "Exporting to " << path << std::endl;
	if (imwrite(path, image) == false) 
	{
		std::cout << "[!] Image could not be saved." << std::endl;
	}
	std::cout << "Image successfully saved!" << std::endl;
}

// Return an adjusted verion of the given Mat image. Right now we only adjust contrast (cts) and 
// brightness (brt), where cts in [0,3.0] and brt in [1,100].
Mat KMR_adjust(Mat image, float adjustments[])
{
	Mat modified_image = image.clone();
	float alpha = adjustments[CONTRAST];
	float betaa = adjustments[BRIGHTNESS];
	// Modify constrast and brightness pixel by pixel.
	// We scan across the x-y, using c to iterate through the number of channels 
	// (i.e RGB has three channels).
	// Using g(x,y)[c] = alpha*g(x,y)[c] + beta
	for (int y = 0; y < image.rows; y++) 
	{
		for (int x = 0; x < image.cols; x++) 
		{
			for (int c = 0; c < image.channels(); c++) 
			{
				modified_image.at<Vec3b>(y,x)[c] = saturate_cast<uchar>(alpha*image.at<Vec3b>(y,x)[c] + betaa);
			}
		}
	}
	return modified_image;
}

// Convert a given Mat image to wxImage. This is a computationally demanding function, therefore 
// only call it when absolutely necessary.
wxImage KMR_mat_to_wx(Mat image) 
{
	Mat image2;
	if (image.channels() == 1) 
	{
		cvtColor(image, image2, COLOR_GRAY2RGB);
	}
	else if (image.channels() == 4) 
	{
		cvtColor(image, image2, COLOR_BGRA2RGB);
	}
	else {
		cvtColor(image, image2, COLOR_BGR2RGB);
	}
	long image_size = image2.rows*image2.cols*image2.channels();
	wxImage wx(image2.cols, image2.rows, (unsigned char*)malloc(image_size), false);
	unsigned char *s = image2.data;
	unsigned char *d = wx.GetData();
	for (long i = 0; i < image_size; i++) 
	{
		d[i] = s[i];
	}
	return wx;
}