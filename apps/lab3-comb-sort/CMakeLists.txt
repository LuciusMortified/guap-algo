cmake_minimum_required(VERSION 4.1)

project(lab3-comb-sort)

add_executable(${PROJECT_NAME} src/main.cpp
        include/comb_sort.h
        include/comb_sort_menu.h)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)