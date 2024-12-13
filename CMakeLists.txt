cmake_minimum_required(VERSION 3.10)
project(SparseSolverLibrary)

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

# Optional: Create example executable for testing
add_executable(solve_example main.cpp)
target_link_libraries(solve_example 
    sparse_solver
    Eigen3::Eigen
)

