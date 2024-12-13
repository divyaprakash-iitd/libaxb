#include <iostream>
#include <fstream>
#include <vector>
#include "sparse_solver.h"

int main() {
    try {
        // Read matrix dimensions
        int rows, cols;
        std::ifstream dimFile("dimensions.txt");
        if (!dimFile.is_open()) {
            std::cerr << "Failed to open dimensions file" << std::endl;
            return 1;
        }
        dimFile >> rows >> cols;
        dimFile.close();

        // Read input files
        std::vector<double> values = SparseSolver::readDoubleFile("values.txt");
        std::vector<int> column_indices = SparseSolver::readIntegerFile("column_indices.txt");
        std::vector<int> row_pointers = SparseSolver::readIntegerFile("row_pointers.txt");
        std::vector<double> rhs = SparseSolver::readDoubleFile("rhs.txt");

        // Initialize the sparse matrix globally
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
            rhs.data(),         // Pointer to right-hand side array
            rhs.size(),         // Size of right-hand side
            solution.data()     // Pointer to solution array
        );

        // Save solution
        std::ofstream solFile("solution.txt");
        if (solFile.is_open()) {
            solFile.precision(12);
            for(const auto& val : solution) {
                solFile << val << "\n";
            }
            solFile.close();
        }

        std::cout << "Solution computed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

