cmake_minimum_required(VERSION 2.6)
project(pawav)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(LibPulse REQUIRED)

# this isn't the best cmake practice (I guess) but it'll do for now
add_executable(pawav src/pawav.c)
target_link_libraries(pawav pulse)
