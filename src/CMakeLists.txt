add_executable(pawav pawav.c reader.c pa_ringbuffer.c
    readerthread.c)
target_link_libraries(pawav m pulse sndfile ${CMAKE_THREAD_LIBS_INIT})
