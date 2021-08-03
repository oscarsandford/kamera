cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project( kamera LANGUAGES CXX )

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source codes.
set(SRCS 
	src/edit.cpp
	src/main.cpp
)

# Add platform-specific bundles.
if (APPLE)
	add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SRCS})
else ()
	# Win32 needed for Windows. Other OS will simply ignore it.
	add_executable(${PROJECT_NAME} WIN32 ${SRCS})
endif()

add_subdirectory(src)
add_subdirectory(external/opencv)
add_subdirectory(external/wxwidgets)


target_include_directories(${PROJECT_NAME} 
	PUBLIC src
	PUBLIC external/opencv/include
	# These next lines are my attempt at retrieving all the include 
	# subdirectories in modules. Will come back to this. 
	PUBLIC external/opencv/modules/core/include
	PUBLIC external/opencv/modules/calib3d/include
	PUBLIC external/opencv/modules/features2d/include
	PUBLIC external/opencv/modules/flann/include
	PUBLIC external/opencv/modules/dnn/include
	PUBLIC external/opencv/modules/highgui/include
	PUBLIC external/opencv/modules/imgcodecs/include
	PUBLIC external/opencv/modules/videoio/include
	PUBLIC external/opencv/modules/imgproc/include
	PUBLIC external/opencv/modules/ml/include
	PUBLIC external/opencv/modules/objdetect/include
	PUBLIC external/opencv/modules/photo/include
	PUBLIC external/opencv/modules/stitching/include
	PUBLIC external/opencv/modules/video/include
	# Build dir has an opencv2 folder with opencv2_modules.hpp that we need.
	PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC external/wxwidgets/include
	# This next line is for setup.h, which is found in the build folder. Might not need this?
	# PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/lib/wx/include/gtk3-unicode-3.1
)

# target_link_directories(${PROJECT_NAME} 
# 	PRIVATE src
# )
set_target_properties(kamera_lib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(${PROJECT_NAME} 
	kamera_lib 
	opencv_core
	wxcore
	wxbase
	wxnet
	# wx::net
	# wx::core 
	# wx::base
)

# TODO: need to figure out jconfig.h