#include <iostream>
#include <opencv2/opencv.hpp>

// Constants to clarify indexing of an array of sliders
#define SLIDER_COUNT 2
#define CONTRAST 0
#define BRIGHTNESS 1

cv::Mat kk_adjust_contrast_and_brightness(cv::Mat, float, float);
cv::Mat kk_import_image(char *);
void kk_export_image(cv::Mat, char *);