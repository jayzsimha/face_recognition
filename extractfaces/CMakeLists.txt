cmake_minimum_required(VERSION 2.8)
project( Workshop )
find_package( OpenCV REQUIRED )
add_executable( extractfaces extractfaces.cpp )
target_link_libraries( extractfaces ${OpenCV_LIBS} )