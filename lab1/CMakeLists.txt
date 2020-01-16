cmake_minimum_required(VERSION 3.5)

project(lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(entry entry/entry.cpp)

add_executable(test_sort
               test_sort/main.cpp)
target_link_libraries(test_sort PRIVATE entry)

add_executable(generate_data
               generate_data/main.cpp)
target_link_libraries(generate_data PRIVATE entry)

file(COPY run.py DESTINATION ${CMAKE_BINARY_DIR})
