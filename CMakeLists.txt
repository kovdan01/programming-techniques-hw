cmake_minimum_required(VERSION 3.5)

project(programming-techniques LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd_party)

set(SQLiteCpp_ROOT_DIR ${THIRD_PARTY_DIR}/SQLiteCpp)
set(SQLiteCpp_INCLUDE_DIR ${SQLiteCpp_ROOT_DIR}/include)
add_subdirectory(${SQLiteCpp_ROOT_DIR})

find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)

set(Entry_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/entry)
set(Entry_INCLUDE_DIR ${Entry_ROOT_DIR})
add_subdirectory(${Entry_ROOT_DIR})

set(Helpers_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/helpers)
set(Helpers_INCLUDE_DIR ${Helpers_ROOT_DIR})
add_subdirectory(${Helpers_ROOT_DIR})

add_subdirectory(generate_data)
add_subdirectory(lab1)
add_subdirectory(lab2)
add_subdirectory(lab3)
add_subdirectory(lab4)
add_subdirectory(lab5)
add_subdirectory(lab6)
