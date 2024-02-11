cmake_minimum_required (VERSION 3.27)
project (simpleCppThreads)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_DEBUG_POSTFIX _d)
# Uncomment below to build and run in Unicode
add_definitions(-DUNICODE -D_UNICODE)
# add the binary tree to the search path for include files
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${CMAKE_SOURCE_DIR}")

 
add_executable(${PROJECT_NAME} main.cpp Runnable.h)
set_target_properties( ${PROJECT_NAME} PROPERTIES  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
