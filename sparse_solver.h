#ifndef SPARSE_SOLVER_H
#define SPARSE_SOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

// C-compatible function declarations for interoperability
void initialize_sparse_matrix(
    const double* values, 
    const int* column_indices, 
    const int* row_pointers,
    int rows, 
    int cols, 
    int nnz
);

void solve_sparse_system(
    const double* rhs,
    int rhs_size,
    double* solution,
    double tolerance,
    int max_iterations
);

#ifdef __cplusplus
}
#endif

#include <vector>
#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

class SparseSolver {
public:
    // Existing C++ class methods remain the same
    static void initialize(
        const double* values, 
        const int* column_indices, 
        const int* row_pointers,
        int rows, 
        int cols, 
        int nnz
    );

    static std::vector<double> solve(
        const double* rhs,
        int rhs_size,
        double* solution,
        double tolerance = 1e-6,
        int max_iterations = 1000
    );

    // Utility functions remain the same
    static std::vector<double> readDoubleFile(const std::string& filename);
    static std::vector<int> readIntegerFile(const std::string& filename);
};

#endif // SPARSE_SOLVER_H

