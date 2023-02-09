cmake_minimum_required(VERSION 3.0)
project(serve)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# src
set(SRC_FILE
    src/fiber.cc
    src/log.cc
    src/mutex.cc
    src/scheduler.cc
    src/thread.cc
    src/timer.cc
    src/utility.cc
    
    
)
# add_library(rleavrs SHARED ${SRC_FILE})

# inc
include_directories(.)

# lib
find_library(PTHREAD pthread)

# exe
add_executable(test_log "test/test_log.cc" ${SRC_FILE})
add_executable(test_utility "test/test_utility.cc" ${SRC_FILE})
add_executable(test_main "test/test_main.cc")
add_executable(test_scheduler "test/test_scheduler.cc" ${SRC_FILE})
