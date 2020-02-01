set(PROJECT_NAME 1_test_sort)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp)
set(HEADERS heap_sort.h
            quick_sort.h
            shaker_sort.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE entry)
target_link_libraries(${PROJECT_NAME} PRIVATE helpers)

file(COPY run.py DESTINATION ${PROJECT_BINARY_DIR})
