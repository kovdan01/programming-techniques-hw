set(PROJECT_NAME 1_test_sort)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp)
set(HEADERS heap_sort.h
            quick_sort.h
            shaker_sort.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${Helpers_INCLUDE_DIR} ${Entry_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE entry)
target_link_libraries(${PROJECT_NAME} PRIVATE helpers)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)

file(COPY run.py DESTINATION ${PROJECT_BINARY_DIR})
