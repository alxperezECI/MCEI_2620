cmake_minimum_required(VERSION 3.16)
 
project(taller_geometria_cgal LANGUAGES CXX)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
 
find_package(CGAL REQUIRED)
 
add_executable(taller_cgal main.cpp)
 
target_link_libraries(taller_cgal PRIVATE CGAL::CGAL)

add_executable(taller_sensores sensores.cpp)
target_link_libraries(taller_sensores PRIVATE CGAL::CGAL)