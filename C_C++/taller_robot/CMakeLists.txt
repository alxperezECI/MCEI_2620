cmake_minimum_required(VERSION 3.20)

project(taller_gsl)


find_package(GSL REQUIRED)


add_executable(cinematica_diferencial cinematica_diferencial.cpp)





target_link_libraries(cinematica_diferencial
    PRIVATE
    GSL::gsl
    GSL::gslcblas
)