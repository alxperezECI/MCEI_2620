cmake_minimum_required(VERSION 3.16)
project(taller_cgal LANGUAGES CXX)

find_package(CGAL REQUIRED)

add_executable(taller_cgal main.cpp)
target_compile_features(taller_cgal PRIVATE cxx_std_17)
target_compile_options(taller_cgal PRIVATE -Wall -Wextra)
target_link_libraries(taller_cgal PRIVATE CGAL::CGAL)

add_executable(casos_especiales casos_especiales.cpp)
target_compile_features(casos_especiales PRIVATE cxx_std_17)
target_compile_options(casos_especiales PRIVATE -Wall -Wextra)
target_link_libraries(casos_especiales PRIVATE CGAL::CGAL)

add_executable(sensores sensores.cpp)
target_compile_features(sensores PRIVATE cxx_std_17)
target_compile_options(sensores PRIVATE -Wall -Wextra)
target_link_libraries(sensores PRIVATE CGAL::CGAL)

add_executable(precision precision.cpp)
target_compile_features(precision PRIVATE cxx_std_17)
target_compile_options(precision PRIVATE -Wall -Wextra)
target_link_libraries(precision PRIVATE CGAL::CGAL)
