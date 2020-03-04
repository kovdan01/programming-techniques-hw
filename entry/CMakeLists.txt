set(LIBRARY_NAME entry)

project(${LIBRARY_NAME} LANGUAGES CXX)

set(SOURCES entry.cpp)
set(HEADERS entry.h)

add_library(${LIBRARY_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${LIBRARY_NAME} PUBLIC ${SQLiteCpp_INCLUDE_DIR})
target_link_libraries(${LIBRARY_NAME} PUBLIC SQLiteCpp sqlite3)
