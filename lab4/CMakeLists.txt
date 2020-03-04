set(PROJECT_NAME 4_prng)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp dummy.cpp custom.cpp)
set(HEADERS chi_squared.h stat_utils.h dummy.h custom.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${Helpers_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE helpers)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::boost)

file(COPY run.py DESTINATION ${PROJECT_BINARY_DIR})
