set(PROJECT_NAME 2_test_search)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp)
set(HEADERS binary_search.h
            linear_search.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} PRIVATE entry)
target_link_libraries(${PROJECT_NAME} PRIVATE helpers)

file(COPY run.py DESTINATION ${PROJECT_BINARY_DIR})
