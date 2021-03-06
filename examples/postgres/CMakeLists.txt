cmake_minimum_required(VERSION 2.8)

find_package(PostgreSQL)

add_definitions(-std=c++11)

include_directories(${CMAKE_BINARY_DIR} ${PostgreSQL_INCLUDE_DIRS})

set(DBBINDER_EXECUTABLE ../../../build/dbbinder)

add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/select.cpp
                    COMMAND ${DBBINDER_EXECUTABLE}
                    ARGS -i ${CMAKE_SOURCE_DIR}/select.sql -o ${CMAKE_BINARY_DIR}/select ${PARAM_DBBINDER_TEMPLATES} ${DBBINDER_EXTRA_ARGS}
                    DEPENDS ${SQLFILE}
                    WORKING_DIRECTORY ${DBBINDER_OUTPUT_PATH}
                    COMMENT dbbinder ${SQLFILE})

add_executable(pgexample "${CMAKE_BINARY_DIR}/select.cpp" main.cpp)

target_link_libraries(pgexample ${PostgreSQL_LIBRARIES})
