find_package(wxWidgets QUIET)

# Add the library to the project if it exists
if (wxWidgets_FOUND)
	message(STATUS "Yay! Found wxWidgets libraries at ${wxWidgets_LIBRARIES}")
	add_library(wxWidgets_lib INTERFACE)
else ()
	message(STATUS "No wxWidgets libraries found.")
	message(STATUS "Downloading and installing wxWidgets in ${STAGED_INSTALL_PREFX}")

	# Download wxWidgets as an external project
	include(ExternalProject)
	ExternalProject_Add(wxWidgets_lib
		GIT_REPOSITORY 
		https://github.com/wxWidgets/wxWidgets.git
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

	set(wxWidgets_ROOT_DIR ${STAGED_INSTALL_PREFIX})
	file(TO_NATIVE_PATH "${wxWidgets_ROOT_DIR}" wxWidgets_ROOT_DIR)
	set(wxWidgets_ROOT_DIR ${wxWidgets_ROOT_DIR} CACHE INTERNAL "wxWidgets installation dir")

	set (ENV_WX_CONFIG ${STAGED_INSTALL_PREFIX}/bin/wx-config)
	file (TO_NATIVE_PATH "${ENV_WX_CONFIG}" ENV_WX_CONFIG)
	set(ENV_WX_CONFIG ${ENV_WX_CONFIG} CACHE INTERNAL "wx-config dir")

endif()