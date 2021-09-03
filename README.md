# Parallel program and sequential for Matrix-Vector multiplication
This program executed on Linux CentOS.
#
* ## Sequential:
This program used here to compare the results (i.e. execution time) with the parallel program.
- To compile this program on the terminal, type this command:
  ```sh
  gcc -o sequential Sequential.c
  ```
  Object file with name sequential will be generated, to execute this file, type this:
  ```sh
  ./sequential
  ```
  #
* ## Parallel:
This program use multiprocessing technique via MPI library, i.e. the execution will be on multiprocessors.
- To compile this program, type this:
  ```sh
  mpicc -o parallel MPI_Parallel.c
  ```
- To execute the object file, type this:
  ```sh
  mpirun -np 4 parallel
  ```
#### _Note:_ Number 4 in the last command represents the number of processors that will be used in the execution of the program.
------------------------------------------------

  Done by:
  ### Eng. Meqdad Darweesh
