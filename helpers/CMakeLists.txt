set(LIBRARY_NAME helpers)

project(${LIBRARY_NAME} LANGUAGES CXX)

set(SOURCES io_operations.cpp)
set(HEADERS io_operations.h)

add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIBRARY_NAME} PUBLIC ${Entry_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC entry)
