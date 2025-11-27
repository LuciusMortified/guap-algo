cmake_minimum_required(VERSION 4.1)
project(lab1-latin-sequence)

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/latin_sequence.h
        include/latin_sequence_menu.h)

target_include_directories(${PROJECT_NAME} PRIVATE include)
