find_package(OpenCV QUIET)

# Add the library to the project if it exists
if (OpenCV_FOUND)
	message(STATUS "Yay! Found OpenCV libraries at ${OpenCV_LIBRARIES}")
	add_library(OpenCV_lib INTERFACE)
else ()
	message(STATUS "No OpenCV libraries found.")
	message(STATUS "Downloading and installing OpenCV in ${STAGED_INSTALL_PREFX}")

	# Download OpenCV as an external project
	include(ExternalProject)
	ExternalProject_Add(OpenCV_lib
		GIT_REPOSITORY 
		https://github.com/opencv/opencv.git
		GIT TAG 
		master 
		UPDATE_COMMAND 
		""
		CMAKE_ARGS
			-DCMAKE_INSTALL_PREFIX=${STAGED_INSTALL_PREFIX}
			-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
			-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
			-DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD}
			-DCMAKE_CXX_EXTENSIONS=${CMAKE_CXX_EXTENSIONS}
			-DCMAKE_CXX_STANDARD_REQUIRED=${CMAKE_CXX_STANDARD_REQUIRED}
			-DwxBUILD_SHARED=OFF
		CMAKE_CACHE_ARGS
			-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
		TEST_AFTER_INSTALL
			0
		DOWNLOAD_NO_PROGRESS
			1
		LOG_CONFIGURE
			1
		LOG_BUILD
			1
		LOG_INSTALL
			1		
	)

endif()