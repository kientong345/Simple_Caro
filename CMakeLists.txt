cmake_minimum_required(VERSION 3.10)
project(simple_caro VERSION 1.0 LANGUAGES CXX)

set(SIMPLE_CARO_NAME simple_caro)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(WORKDIR ${CMAKE_SOURCE_DIR})

set(SOURCES
    ${WORKDIR}/simple_caro/src/board_context.cpp
    ${WORKDIR}/simple_caro/src/player_context.cpp
    ${WORKDIR}/simple_caro/src/game_judge.cpp
    ${WORKDIR}/simple_caro/src/ruling.cpp
    ${WORKDIR}/simple_caro/src/utils.cpp
    ${WORKDIR}/simple_caro/src/simple_caro.cpp
)

add_library(${SIMPLE_CARO_NAME} STATIC ${SOURCES})

target_include_directories(${SIMPLE_CARO_NAME} PUBLIC "${WORKDIR}/simple_caro/include")

file(COPY "${WORKDIR}/cli_tool/" DESTINATION "${CMAKE_BINARY_DIR}/cli_tool/")
file(COPY "${WORKDIR}/example/" DESTINATION "${CMAKE_BINARY_DIR}/example/")
file(COPY "${WORKDIR}/wrapper/" DESTINATION "${CMAKE_BINARY_DIR}/wrapper/")

install(TARGETS ${SIMPLE_CARO_NAME}
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(
    DIRECTORY "${WORKDIR}/simple_caro/include/"
    DESTINATION include/simple_caro
)

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()