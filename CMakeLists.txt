cmake_minimum_required(VERSION 3.9)
project(cvPerceptualHash)

set(CMAKE_CXX_STANDARD 11)
find_package(OpenCV REQUIRED)


#add_library(cvPerceptualHash library.cpp library.h MathUtils.cpp)
#target_link_libraries(cvPerceptualHash ${OpenCV_LIBS})

add_executable(aa main.cpp cvPerceptualHash.cpp cvPerceptualHash.h)
target_link_libraries(aa ${OpenCV_LIBS})