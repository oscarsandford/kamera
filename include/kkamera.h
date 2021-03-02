#include <opencv2/opencv.hpp>

// Constants to clarify indexing of an array of sliders
#define SLIDER_COUNT 2
#define CONTRAST 0
#define BRIGHTNESS 1

cv::Mat adjust_contrast_and_brightness(cv::Mat, float, float);
cv::Mat import_image(char *);
void export_image(cv::Mat, char *);