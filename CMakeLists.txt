cmake_minimum_required(VERSION 2.6)
project(pawav)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
SET(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -pedantic -ggdb3")

find_package(LibPulse REQUIRED)
find_package(SndFile REQUIRED)

# this isn't the best cmake practice (I guess) but it'll do for now
add_executable(pawav src/pawav.c src/reader.c)
target_link_libraries(pawav pulse sndfile)
