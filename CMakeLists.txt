cmake_minimum_required(VERSION 3.10)
project(SparseSolverLibrary LANGUAGES CXX Fortran)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compilation flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -O3 -march=native -DNDEBUG")

# Find Eigen package
find_package(Eigen3 REQUIRED)

# Create library
add_library(sparse_solver SHARED 
    sparse_solver.cpp
)

# Set include directories for Eigen
target_include_directories(sparse_solver PUBLIC 
    /usr/include/eigen3
)

# Link Eigen and OpenMP
target_link_libraries(sparse_solver 
    Eigen3::Eigen
    OpenMP::OpenMP_CXX
)

# Find OpenMP package
find_package(OpenMP REQUIRED)

# Create Fortran executable
add_executable(solve_fortran 
    sparse_solver_interface.f90
    fortran_main.f90
)

target_link_libraries(solve_fortran 
    sparse_solver
    Eigen3::Eigen
    OpenMP::OpenMP_CXX
)

