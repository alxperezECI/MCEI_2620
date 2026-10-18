cmake_minimum_required( VERSION 3.20 )
project(matrices)
find_package( Eigen3 )
add_executable( ${PROJECT_NAME} punto4_inversa_qr_svd.cpp )

