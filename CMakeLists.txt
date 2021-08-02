cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project( kamera LANGUAGES CXX )

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Required third party libraries.
add_subdirectory(external/wxwidgets)
add_subdirectory(external/opencv)

# Source codes.
set(SRCS 
	src/edit.cpp
	src/main.cpp
)

# Add platform-specific bundles.
if (APPLE)
	add_executable(kamera MACOSX_BUNDLE ${SRCS})
else ()
	# Win32 needed for Windows. Other OS will simply ignore it.
	add_executable(kamera WIN32 ${SRCS})
endif()

target_include_directories(${PROJECT_NAME} 
	PUBLIC include
	PUBLIC external/wxwidgets/include
	PUBLIC external/opencv/include
)

target_link_directories(${PROJECT_NAME} 
	PRIVATE src
	PRIVATE external/wxwidgets/src
	PRIVATE external/opencv/src
)

target_link_libraries(${PROJECT_NAME} SRCS opencv wxwidgets)
