cmake_minimum_required(VERSION 3.10)
project(SingletonBase)

add_library(SingletonBase INTERFACE)
target_include_directories(SingletonBase 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(PM::SingletonBase ALIAS SingletonBase)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_subdirectory(tests)
endif()