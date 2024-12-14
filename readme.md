# libaxb - A Library for Solving Sparse Linear Systems

`libaxb` is a C++ library with C++ and Fortran interfaces for solving sparse linear systems of the form Ax = b, where A is a sparse matrix stored in Compressed Row Storage (CRS) format. The library uses the Eigen library's BiCGSTAB solver with an Incomplete LU preconditioner.

## Features

- Efficient sparse matrix construction and storage using Eigen library
- Optimized BiCGSTAB solver with ILU preconditioner
- OpenMP parallelization support
- Dual interface supporting both C++ and Fortran applications
- Global storage of sparse matrix for multiple solves with different RHS vectors

## Prerequisites

- CMake (>= 3.10)
- C++ compiler with OpenMP support
- Fortran compiler (for Fortran interface)
- Eigen3 library
- Linux environment (tested on Ubuntu)

### Installing Prerequisites on Ubuntu

```bash
sudo apt-get update
sudo apt-get install cmake g++ gfortran libeigen3-dev
```

## Input File Format

The library expects input files in the following format:

- `dimensions.txt`: Two integers specifying number of rows and columns of the matrix
- `values.txt`: Non-zero values of the matrix in CRS format
- `column_indices.txt`: Column indices for each non-zero value
- `row_pointers.txt`: Row pointer array in CRS format
- `rhs.txt`: Right-hand side vector b

## Using the Library

### In C++

```cpp
#include "sparse_solver.h"
#include <vector>

// Read input files using utility functions
std::vector<double> values = SparseSolver::readDoubleFile("values.txt");
std::vector<int> column_indices = SparseSolver::readIntegerFile("column_indices.txt");
std::vector<int> row_pointers = SparseSolver::readIntegerFile("row_pointers.txt");
std::vector<double> rhs = SparseSolver::readDoubleFile("rhs.txt");

// Initialize sparse matrix
SparseSolver::initialize(
    values.data(),
    column_indices.data(),
    row_pointers.data(),
    rows,
    cols,
    values.size()
);

// Prepare solution vector
std::vector<double> solution(rhs.size());

// Solve the system
SparseSolver::solve(
    rhs.data(),
    rhs.size(),
    solution.data()
);
```

### In Fortran

```fortran
use sparse_solver_interface
use iso_c_binding

! Declare arrays
real(c_double), allocatable :: values(:)
integer(c_int), allocatable :: column_indices(:)
integer(c_int), allocatable :: row_pointers(:)
real(c_double), allocatable :: rhs(:)
real(c_double), allocatable :: solution(:)

! After reading data into arrays...

! Initialize matrix
call initialize_sparse_matrix(values, column_indices, row_pointers, rows, cols, nnz)

! Solve system
call solve_sparse_system(rhs, rhs_size, solution, 1.0d-6, 1000)
```

## Building the Library

```bash
mkdir build
cd build
cmake ..
make
```

This will create:
- `libsparse_solver.so`: The shared library
- `solve_example`: C++ example executable
- `solve_fortran`: Fortran example executable

## Linking with Your Project

### For C++ Projects

In your CMakeLists.txt:
```cmake
find_package(Eigen3 REQUIRED)
find_package(OpenMP REQUIRED)

target_link_libraries(your_executable 
    sparse_solver
    Eigen3::Eigen
    OpenMP::OpenMP_CXX
)
```

### For Fortran Projects

In your Makefile:
```makefile
LIB_DIR = -L/path/to/libaxb/build
LIB_L = -lsparse_solver
LDFLAGS = $(LIB_DIR) $(LIB_L)
```

Add to your .bashrc:
```bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/libaxb/build
```

## Solver Parameters

- Default tolerance: 1e-6
- Default maximum iterations: 1000
- ILU preconditioner drop tolerance: 1e-4

## Example Files

The repository includes complete examples:
- `main.cpp`: C++ implementation example
- `fortran_main.f90`: Fortran implementation example
- `sparse_solver_interface.f90`: Fortran interface module

## Directory Structure
```
libaxb/
├── CMakeLists.txt
├── main.cpp
├── fortran_main.f90
├── sparse_solver.cpp
├── sparse_solver.h
└── sparse_solver_interface.f90
```

