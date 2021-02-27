#include <opencv2/opencv.hpp>
cv::Mat adjust_contrast_and_brightness(cv::Mat, float, float);
cv::Mat import_image(char *);
void export_image(cv::Mat, char *);