cmake_minimum_required(VERSION 3.8)
set(CMAKE_C_STANDARD 11)

project(work)
aux_source_directory(. SRC_LIST)
include_directories(include/)
add_subdirectory(Calibrater/)
add_subdirectory(CapturePlayer/)
# add_compile_options(-L lib/lib -l*)
# add_compile_options(-L lib/bin -l*)

FIND_PACKAGE( OpenCV REQUIRED )
# INCLUDE_DIRECTORIES( ${OpenCV_INCLUDE_DIRS} )

# TARGET_LINK_LIBRARIES (word ${OpenCV_LIBS})         

add_executable(work ${SRC_LIST})
target_link_libraries(work Calibrater CapturePlayer ${OpenCV_LIBS})

