cmake_minimum_required(VERSION 4.1)
project(lab2-hash-table)

add_executable(${PROJECT_NAME}
        src/main.cpp
        include/hash_key.h
        include/hash_table.h
        include/hash_bucket.h
        include/hash_table_menu.h)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_include_directories(${PROJECT_NAME} PRIVATE include)
