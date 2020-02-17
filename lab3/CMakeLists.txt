set(PROJECT_NAME 3_hashes)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp
            dummy.cpp
            elf.cpp
            rot13.cpp)
set(HEADERS dummy.h
            elf.h
            rot13.h
            hash_table.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${Helpers_INCLUDE_DIR} ${Entry_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE entry)
target_link_libraries(${PROJECT_NAME} PRIVATE helpers)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)

file(COPY run.py DESTINATION ${PROJECT_BINARY_DIR})
