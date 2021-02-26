cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
# Core source
project(kamera_core LANGUAGES CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if (DEFINED ENV_WX_CONFIG)
	set(ENV{WX_CONFIG} ${ENV_WX_CONFIG})
endif()

set(wxWidgets_USE_STATIC 1)

find_package(wxWidgets REQUIRED)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(SRCS 
	gui.cpp
	main.cpp
)

include(${wxWidgets_USE_FILE})

if (APPLE)
	# MacOS bundle
	add_executable(kamera MACOSX_BUNDLE ${SRCS})
else ()
	# Win32 needed for Windows. Other OS will simply ignore it.
	add_executable(kamera WIN32 ${SRCS})
endif()

# Link third party libraries
target_link_libraries(kamera PRIVATE 
	${wxWidgets_LIBRARIES} 
	${OpenCV_LIBS}
)