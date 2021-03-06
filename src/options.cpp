#include <kmrutils.h>

// Read in and return a Mat image of the file located at path.
cv::Mat KMR_import(char *path) 
{
	std::cout << "Importing from " << path << std::endl;
	cv::Mat image = cv::imread(path, 1);
	if (!image.data) 
	{
		std::cout << "[!] No image data." << std::endl;
		return cv::Mat{};	
	}
	return image;
}

// Write a Mat image to the path defined by path.
void KMR_export(cv::Mat image, char *path) 
{
	std::cout << "Exporting to " << path << std::endl;
	if (cv::imwrite(path, image) == false) 
	{
		std::cout << "[!] Image could not be saved." << std::endl;
	}
	std::cout << "Image successfully saved!" << std::endl;
}