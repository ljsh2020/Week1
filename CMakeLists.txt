cmake_minimum_required( VERSION 3.15 )

project( find_apple )

find_package( OpenCV 4 REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIR} )

add_executable( find_apple find_apple.cpp )

target_link_libraries( find_apple  ${OpenCV_LIBS} )
