cmake_minimum_required(VERSION 3.10)
project(SparseSolverLibrary LANGUAGES CXX Fortran)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Eigen package
find_package(Eigen3 REQUIRED)

# Create library
add_library(sparse_solver SHARED 
    sparse_solver.cpp
)

# Link Eigen
target_link_libraries(sparse_solver 
    Eigen3::Eigen
)

# Create Fortran executable
add_executable(solve_fortran fortran_main.f90)
target_link_libraries(solve_fortran 
    sparse_solver
    Eigen3::Eigen
)

