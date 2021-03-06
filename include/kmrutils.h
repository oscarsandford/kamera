// Define top-level function prototypes and dependencies.
#include <iostream>
// OpenCV is used nearly throughout.
#include <opencv2/opencv.hpp>

// These could just as well be defined in editor.h, but they 
// are better organized here.
cv::Mat KMR_import(char *);
void KMR_export(cv::Mat, char *);