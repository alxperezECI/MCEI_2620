cmake_minimum_required(VERSION 3.20)

project(taller_gsl)


find_package(GSL REQUIRED)


add_executable(punto_1 punto_1.cpp)


add_executable(punto_2 punto_2.cpp)


add_executable(diferenciacion diferenciacion.cpp)


target_link_libraries(punto_1
    PRIVATE
    GSL::gsl
    GSL::gslcblas
)


target_link_libraries(punto_2
    PRIVATE
    GSL::gsl
    GSL::gslcblas
)

target_link_libraries(diferenciacion
    PRIVATE
    GSL::gsl
    GSL::gslcblas
)