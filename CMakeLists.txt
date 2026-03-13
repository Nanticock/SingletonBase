cmake_minimum_required(VERSION 3.10)
project(SingletonBase)

add_library(SingletonBase INTERFACE
    include/PM/SingletonBase.h
    include/PM/ScopedSingletonState.h
)
target_include_directories(SingletonBase 
    INTERFACE 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(PM::SingletonBase ALIAS SingletonBase)

# By default tests are disabled unless this CMakeLists file is opened as a standalone project and catch2 is found
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    find_package(Catch2)
    set(default_tests ${Catch2_FOUND})
else()
    set(default_tests OFF)
endif()

option(SINGLETON_BASE_BUILD_TESTS "Build tests for SingletonBase" ${default_tests})

if(SINGLETON_BASE_BUILD_TESTS)
    add_subdirectory(tests)
endif()
