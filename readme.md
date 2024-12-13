I'll modify the CMakeLists.txt to include these compilation flags:

```cmake
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

```

The key changes are:
1. Added `-fopenmp -O3 -march=native -DNDEBUG` flags
2. Added explicit include path for Eigen3
3. Added OpenMP package requirement and linking
4. Ensured both library and executable get the OpenMP flags

Build as before:
```bash
mkdir build
cd build
cmake ..
make
```

Or if you prefer to use g++ directly for just the library:
```bash
g++ -shared -fPIC -fopenmp -O3 -march=native -DNDEBUG -I /usr/include/eigen3 sparse_solver.cpp -o libsparse_solver.so
```

Then compile the Fortran code with:
```bash
gfortran sparse_solver_interface.f90 fortran_main.f90 -L. -lsparse_solver -I/usr/include/eigen3 -o solve_fortran
```

