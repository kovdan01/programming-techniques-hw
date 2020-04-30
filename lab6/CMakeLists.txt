set(PROJECT_NAME 6_patterns)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp command.cpp receiver.cpp invoker.cpp)
set(HEADERS command.h receiver.h invoker.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
