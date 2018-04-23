cmake_minimum_required(VERSION 3.9)
project(cvPerceptualHash)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)

add_library(cvPerceptualHash cvPerceptualHash.cpp cvPerceptualHash.h MathUtils.cpp)
target_link_libraries(cvPerceptualHash ${OpenCV_LIBS})