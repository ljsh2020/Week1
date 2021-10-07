cmake_minimum_required( VERSION 3.15 )

project( question2 )

find_package( OpenCV 4 REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIR} )

add_executable( question2 question2.cpp )

target_link_libraries( question2  ${OpenCV_LIBS} )
