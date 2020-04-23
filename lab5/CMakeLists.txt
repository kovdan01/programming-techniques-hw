set(PROJECT_NAME 5_patterns)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp composite.cpp)
set(HEADERS composite.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
