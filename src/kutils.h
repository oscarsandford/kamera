// Top level header.
#include <wx/wx.h>
#include <wx/image.h>
#include <opencv2/opencv.hpp>

#define SLIDER_COUNT 2
#define CONTRAST 0
#define BRIGHTNESS 1

cv::Mat KMR_import(char *);
void KMR_export(cv::Mat, char *);

cv::Mat KMR_adjust(cv::Mat, float[]);
wxImage KMR_mat_to_wx(cv::Mat);