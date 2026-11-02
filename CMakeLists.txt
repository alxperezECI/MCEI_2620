cmake_minimum_required(VERSION 3.10)
project(Practico LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 REQUIRED)

# 1. Primero creamos el ejecutable
add_executable(part1 part1.cpp)
target_link_libraries(part1 PRIVATE Eigen3::Eigen)

# 2. AL FINAL, agregamos el comando para copiar el CSV
add_custom_command(TARGET part1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/datos_convertidor_realista.csv
    ${CMAKE_BINARY_DIR}/datos_convertidor_realista.csv
)