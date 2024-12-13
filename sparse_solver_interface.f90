module sparse_solver_interface
    use, intrinsic :: iso_c_binding
    implicit none

    ! Interface block for C functions
    interface
        subroutine initialize_sparse_matrix(values, column_indices, row_pointers, rows, cols, nnz) bind(c)
            import :: c_double, c_int
            real(c_double), intent(in) :: values(*)
            integer(c_int), intent(in) :: column_indices(*)
            integer(c_int), intent(in) :: row_pointers(*)
            integer(c_int), value :: rows
            integer(c_int), value :: cols
            integer(c_int), value :: nnz
        end subroutine initialize_sparse_matrix

        subroutine solve_sparse_system(rhs, rhs_size, solution, tolerance, max_iterations) bind(c)
            import :: c_double, c_int
            real(c_double), intent(in) :: rhs(*)
            integer(c_int), value :: rhs_size
            real(c_double), intent(out) :: solution(*)
            real(c_double), value :: tolerance
            integer(c_int), value :: max_iterations
        end subroutine solve_sparse_system
    end interface

end module sparse_solver_interface

