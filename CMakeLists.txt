cmake_minimum_required(VERSION 3.9)

# name of this project
project(CppOpenCvArProject)

# Enable C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# set OpenCV_DIR variable equal to the path to the cmake
# files within the previously installed opencv program
set(OpenCV_DIR /opt/homebrew/Cellar/opencv/4.6.0_1/lib/cmake/opencv4)

# configure the necessary common CMake environment variables
# needed to include and link the OpenCV program into this
# demo project, namely OpenCV_INCLUDE_DIRS and OpenCV_LIBS
find_package( OpenCV REQUIRED )

# tell the build to include the headers from OpenCV
include_directories( ${OpenCV_INCLUDE_DIRS} )

# specify the executable target to be built
add_executable(CppOpenCvArProject main.cpp Tools.cpp Tools.h)

# tell it to link the executable target against OpenCV
target_link_libraries(CppOpenCvArProject ${OpenCV_LIBS} )

