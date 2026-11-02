cmake_minimum_required(VERSION 3.10)
project(TallerMetodos CXX)

# Configurar C++ estándar
set(CMAKE_CXX_STANDARD 17)

# Buscar las librerías necesarias de GSL (GNU Scientific Library)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSL REQUIRED gsl)

# Definir explícitamente el ejecutable
add_executable(ejecutable main.cpp)

# Conectar las cabeceras y librerías de GSL
target_include_directories(ejecutable PRIVATE ${GSL_INCLUDE_DIRS})
target_link_directories(ejecutable PRIVATE ${GSL_LIBRARY_DIRS})
target_link_libraries(ejecutable PRIVATE ${GSL_LIBRARIES})