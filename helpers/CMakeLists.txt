set(LIBRARY_NAME helpers)

project(${LIBRARY_NAME} LANGUAGES CXX)

set(SOURCES io_operations.cpp)
set(HEADERS io_operations.h)

add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})
