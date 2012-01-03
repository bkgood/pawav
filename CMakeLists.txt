cmake_minimum_required(VERSION 2.6)
project(pawav)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_C_FLAGS "-std=gnu99 -Wall -Wextra -pedantic -ggdb3")
set(CMAKE_THREAD_PREFER_PTHREAD true)

find_package(Threads REQUIRED)
find_package(LibPulse REQUIRED)
find_package(SndFile REQUIRED)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM)
endif(DOXYGEN_FOUND)

if(NOT CMAKE_USE_PTHREADS_INIT)
    message(SEND_ERROR "pthreads not found")
endif(NOT CMAKE_USE_PTHREADS_INIT)

add_subdirectory(src)
