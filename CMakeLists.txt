cmake_minimum_required(VERSION 2.6)
project(pawav)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -pedantic -ggdb3")
set(CMAKE_THREAD_PREFER_PTHREAD true)

find_package(Threads REQUIRED)
find_package(LibPulse REQUIRED)
find_package(SndFile REQUIRED)

if(NOT CMAKE_USE_PTHREADS_INIT)
    message(SEND_ERROR "pthreads not found")
endif(NOT CMAKE_USE_PTHREADS_INIT)

# this isn't the best cmake practice (I guess) but it'll do for now
add_executable(pawav src/pawav.c src/reader.c src/pa_ringbuffer.c
    src/readerthread.c)
target_link_libraries(pawav pulse sndfile ${CMAKE_THREAD_LIBS_INIT})
