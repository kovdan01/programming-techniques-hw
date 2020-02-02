set(PROJECT_NAME generate_data)

project(${PROJECT_NAME} LANGUAGES CXX)

set(SOURCES main.cpp)
set(HEADERS geography.h
            names.h
            teams.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${Entry_INCLUDE_DIR} ${SQLiteCpp_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE entry)
