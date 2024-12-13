#ifndef SPARSE_SOLVER_H
#define SPARSE_SOLVER_H

#include <vector>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

class SparseSolver {
public:
    // Initialize function to create and store sparse matrix globally
    static void initialize(
        const double* values, 
        const int* column_indices, 
        const int* row_pointers,
        int rows, 
        int cols, 
        int nnz
    );

    // Solve function that uses the globally stored matrix
    static std::vector<double> solve(
        const double* rhs,
        int rhs_size,
        double* solution,
        double tolerance = 1e-6,
        int max_iterations = 1000
    );

    // Utility functions (optional)
    static std::vector<double> readDoubleFile(const std::string& filename);
    static std::vector<int> readIntegerFile(const std::string& filename);
};

#endif // SPARSE_SOLVER_H

