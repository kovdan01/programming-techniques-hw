cmake_minimum_required(VERSION 3.5)

project(programming-techniques LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(THIRD_PARTY_DIR ${CMAKE_CURRENT_LIST_DIR}/3rd_party/)

set(SQLiteCpp_ROOT_DIR ${THIRD_PARTY_DIR}/SQLiteCpp/)
set(SQLiteCpp_INCLUDE_DIR ${SQLiteCpp_ROOT_DIR}/include/)
add_subdirectory(${SQLiteCpp_ROOT_DIR})

set(Entry_ROOT_DIR entry)
set(Entry_INCLUDE_DIR ${Entry_ROOT_DIR})
add_subdirectory(${Entry_ROOT_DIR})

include_directories(${SQLiteCpp_INCLUDE_DIR}
                    ${Entry_INCLUDE_DIR})

set(Helpers_ROOT_DIR helpers)
set(Helpers_INCLUDE_DIR ${Helpers_ROOT_DIR})
add_subdirectory(${Helpers_ROOT_DIR})

include_directories(${Helpers_INCLUDE_DIR})

add_subdirectory(generate_data)
add_subdirectory(lab1)
add_subdirectory(lab2)
