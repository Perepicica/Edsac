cmake_minimum_required(VERSION 3.9)
project(Edsac)

set(CMAKE_CXX_STANDARD 11)

add_executable(Edsac input.h running.h check_data.c output.c input.c running.c)