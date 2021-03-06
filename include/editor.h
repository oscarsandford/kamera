// Function prototypes and constant definitions for any editor-based 
// features. Function behavior is fully defined in the appropraite source files.

// wxWidgets for GUI
#include<wx/wx.h>
// Top-level dependencies
#include<kmrutils.h>

#define SLIDER_COUNT 2
#define CONTRAST 0
#define BRIGHTNESS 1

cv::Mat KMR_adjust(cv::Mat, float[]);
wxImage KMR_mat_to_wx(cv::Mat);