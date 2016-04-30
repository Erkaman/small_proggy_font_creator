######################################
#### PROJECT SETTINGS.
######################################

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)
PROJECT(small_proggy_font_creator)


######################################
#### SET SOURCE FILES.
######################################

file(GLOB SRC src/*.cpp)
include_directories("src")

######################################
############ MAKE EXECUTABLE
######################################

add_executable (small_proggy_font_creator ${SRC})
