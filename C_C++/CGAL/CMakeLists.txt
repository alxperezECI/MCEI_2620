cmake_minimum_required(VERSION 3.16)
project(taller_cgal LANGUAGES CXX)

find_package(CGAL REQUIRED)

# Ejercicio de Sensores, pregunta de investigación 2
add_executable(ejercicio_sensores ejercicio_sensores.cpp)
target_link_libraries(ejercicio_sensores PRIVATE CGAL::CGAL)

# ejericio_practico.cpp
add_executable(taller_cgal ejercicio_practico.cpp)
target_link_libraries(taller_cgal PRIVATE CGAL::CGAL)

# Experimento 1: Puntos repetidos
add_executable(taller_repetidos puntos_repetidos.cpp)
target_link_libraries(taller_repetidos PRIVATE CGAL::CGAL)

# Experimento 2: Puntos colineales
add_executable(taller_colineales puntos_colineales.cpp)
target_link_libraries(taller_colineales PRIVATE CGAL::CGAL)

# Experimento 3: Puntos cocirculares
add_executable(taller_cocirculares puntos_cocirculares.cpp)
target_link_libraries(taller_cocirculares PRIVATE CGAL::CGAL)

