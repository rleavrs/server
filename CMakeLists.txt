cmake_minimum_required(VERSION 3.0)
project(serve)

add_definitions(-w)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# src
set(SRC_FILE
    src/address.cc
    src/bytearray.cc
    src/fd_manager.cc    
    src/fiber.cc
    src/hook.cc
    src/iomanager.cc
    src/log.cc
    src/mutex.cc
    src/scheduler.cc
    src/socket.cc
    src/thread.cc
    src/timer.cc
    src/utility.cc
    
    
)
# add_library(rleavrs SHARED ${SRC_FILE})

# inc
include_directories(.)

# lib
find_library(PTHREAD pthread)
find_package(OpenSSL REQUIRED)

set(LIBS
    ${OPENSSL_LIBRARIES}
    )

# exe
add_executable(test_log "test/test_log.cc" ${SRC_FILE})
target_link_libraries(test_log ${LIBS})

add_executable(test_utility "test/test_utility.cc" ${SRC_FILE})
target_link_libraries(test_utility ${LIBS})

add_executable(test_main "test/test_main.cc" ${SRC_FILE})
target_link_libraries(test_main ${LIBS})

add_executable(test_scheduler "test/test_scheduler.cc" ${SRC_FILE})
target_link_libraries(test_scheduler ${LIBS})

add_executable(test_timers "test/test_timer.cc" ${SRC_FILE})
target_link_libraries(test_timers ${LIBS})
