cmake_minimum_required(VERSION 3.15)
project(StrokeProcessing)

set(CMAKE_CXX_STANDARD 17)
# Boost
find_package(Boost 1.65.1 REQUIRED COMPONENTS filesystem )

# Opencv
find_package(OpenCV 4.2.0 REQUIRED core imgproc imgcodecs highgui ximgproc)

# Eigen3
find_package(Eigen3 REQUIRED)

include_directories(include/)
include_directories( ${OpenCV_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${EIGEN3_INCLUDE_DIR})


add_executable(StrokeProcessing main.cpp stroke_transformer.cpp stroke_transformer.h imgProcessing.cpp imgProcessing.h)

target_include_directories( ${PROJECT_NAME}
        PUBLIC ${Boost_INCLUDE_DIRS}
        PUBLIC ${OpenCV_INCLUDE_DIRS} )

target_link_libraries( StrokeProcessing ${OpenCV_LIBS}
                                        ${Boost_LIBRARIES} )