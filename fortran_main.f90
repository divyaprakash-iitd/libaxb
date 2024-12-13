program sparse_solver_fortran
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

    ! Declare variables
    integer :: rows, cols, nnz, i, io_stat
    character(100) :: line
    real(c_double), allocatable :: values(:)
    integer(c_int), allocatable :: column_indices(:), row_pointers(:)
    real(c_double), allocatable :: rhs(:), solution(:)
    real(c_double) :: temp_real

    ! Read matrix dimensions
    open(unit=10, file='dimensions.txt', status='old')
    read(10, *) rows, cols
    close(10)
    print *, "Matrix dimensions:", rows, "x", cols

    ! Count number of values (nnz)
    open(unit=11, file='values.txt', status='old')
    nnz = 0
    do 
        read(11, *, end=100) 
        nnz = nnz + 1
    end do
    100 continue
    print *, "Number of non-zero elements:", nnz
    
    ! Allocate arrays
    allocate(values(nnz))
    allocate(column_indices(nnz))
    allocate(row_pointers(rows+1))
    allocate(rhs(rows))
    allocate(solution(rows))

    ! Read values
    rewind(11)
    do i = 1, nnz
        read(11, *) values(i)
    end do
    close(11)

    ! Read column indices
    open(unit=12, file='column_indices.txt', status='old')
    do i = 1, nnz
        read(12, *, iostat=io_stat) temp_real
        if (io_stat /= 0) then
            print *, "Error reading column index at position:", i
            stop
        endif
        column_indices(i) = int(temp_real)  ! Convert to integer
    end do
    close(12)

    ! Read row pointers
    open(unit=13, file='row_pointers.txt', status='old')
    do i = 1, rows+1
        read(13, *, iostat=io_stat) temp_real
        if (io_stat /= 0) then
            print *, "Error reading row pointer at position:", i
            stop
        endif
        row_pointers(i) = int(temp_real)  ! Convert to integer
    end do
    close(13)

    ! Read right-hand side
    open(unit=14, file='rhs.txt', status='old')
    do i = 1, rows
        read(14, *) rhs(i)
    end do
    close(14)

    ! Print some debug information
    print *, "First few values:", values(1:min(5,nnz))
    print *, "First few column indices:", column_indices(1:min(5,nnz))
    print *, "First few row pointers:", row_pointers(1:min(5,rows+1))
    print *, "First few RHS entries:", rhs(1:min(5,rows))

    ! Call C++ library functions
    print *, "Initializing sparse matrix..."
    call initialize_sparse_matrix(values, column_indices, row_pointers, rows, cols, nnz)
    
    print *, "Solving system..."
    call solve_sparse_system(rhs, rows, solution, 1.0d-6, 1000)

    ! Write solution to file
    open(unit=15, file='solution_fortran.txt', status='replace')
    do i = 1, rows
        write(15, '(ES25.16)') solution(i)
    end do
    close(15)

    print *, "Solution written to solution_fortran.txt"
    print *, "First few solution entries:", solution(1:min(5,rows))

    ! Clean up
    deallocate(values, column_indices, row_pointers, rhs, solution)

end program sparse_solver_fortran

