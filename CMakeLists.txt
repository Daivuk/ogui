cmake_minimum_required(VERSION 3.0.0)

project(libogui)

if(WIN32)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

file(GLOB ogui_cpp_files
    ./src/*.*
)
file(GLOB ogui_h_files
    ./include/ogui/*.*
)

list(APPEND ogui_src_files ${ogui_cpp_files} ${ogui_h_files})

add_library(${PROJECT_NAME} STATIC ${ogui_src_files})
target_include_directories(${PROJECT_NAME} PUBLIC ./include PRIVATE ./src)
