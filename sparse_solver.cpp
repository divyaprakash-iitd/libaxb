#include "sparse_solver.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <cstring>

// Existing implementation remains the same...

// C-compatible wrapper functions
void initialize_sparse_matrix(
    const double* values,
    const int* column_indices,
    const int* row_pointers,
    int rows,
    int cols,
    int nnz
) {
    SparseSolver::initialize(values, column_indices, row_pointers, rows, cols, nnz);
}

void solve_sparse_system(
    const double* rhs,
    int rhs_size,
    double* solution,
    double tolerance,
    int max_iterations
) {
    SparseSolver::solve(rhs, rhs_size, solution, tolerance, max_iterations);
}


// Global variables to store the matrix and its dimensions
namespace {
    Eigen::SparseMatrix<double, Eigen::RowMajor> global_A;
    bool matrix_initialized = false;
}

void SparseSolver::initialize(
    const double* values, 
    const int* column_indices, 
    const int* row_pointers,
    int rows, 
    int cols, 
    int nnz
) {
    // Create triplet list
    typedef Eigen::Triplet<double> T;
    std::vector<T> tripletList;
    tripletList.reserve(nnz);
    
    for(int i = 0; i < rows; i++) {
        for(int j = row_pointers[i]; j < row_pointers[i+1]; j++) {
            tripletList.emplace_back(i, column_indices[j], values[j]);
        }
    }
    
    // Create and store RowMajor sparse matrix
    global_A = Eigen::SparseMatrix<double, Eigen::RowMajor>(rows, cols);
    global_A.setFromTriplets(tripletList.begin(), tripletList.end());
    global_A.makeCompressed();

    // Mark matrix as initialized
    matrix_initialized = true;
}

std::vector<double> SparseSolver::solve(
    const double* rhs,
    int rhs_size,
    double* solution,
    double tolerance,
    int max_iterations
) {
    // Check if matrix is initialized
    if (!matrix_initialized) {
        throw std::runtime_error("Matrix not initialized. Call initialize() first.");
    }

    // Convert rhs to Eigen vector
    Eigen::VectorXd b_eigen = Eigen::Map<const Eigen::VectorXd>(rhs, rhs_size);
    
    // Create solver with preconditioner
    Eigen::BiCGSTAB<Eigen::SparseMatrix<double, Eigen::RowMajor>, Eigen::IncompleteLUT<double>> solver;
    
    solver.setTolerance(tolerance);
    solver.setMaxIterations(max_iterations);
    solver.preconditioner().setDroptol(1e-4);
    
    // Compute and solve
    solver.compute(global_A);
    Eigen::VectorXd x = solver.solve(b_eigen);
    
    // Check solve status
    if(solver.info() != Eigen::Success) {
        throw std::runtime_error("Solver failed to converge");
    }
    
    // Copy solution to provided array
    std::memcpy(solution, x.data(), x.size() * sizeof(double));

    // Also return as vector for flexibility
    return std::vector<double>(x.data(), x.data() + x.size());
}

// Existing utility functions (readDoubleFile, readIntegerFile) remain the same
std::vector<int> SparseSolver::readIntegerFile(const std::string& filename) {
    std::vector<int> data;
    std::ifstream file(filename);
    double value;  
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    while (file >> value) {
        data.push_back(static_cast<int>(std::round(value)));
    }
    
    file.close();
    return data;
}

std::vector<double> SparseSolver::readDoubleFile(const std::string& filename) {
    std::vector<double> data;
    std::ifstream file(filename);
    double value;
    
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    while (file >> value) {
        data.push_back(value);
    }
    
    file.close();
    return data;
}
