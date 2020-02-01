project(entry LANGUAGES CXX)

set(SOURCES_ENTRY entry.cpp)
set(HEADERS_ENTRY entry.h)

add_library(entry ${SOURCES_ENTRY} ${HEADERS_ENTRY})
