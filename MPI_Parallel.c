/*
	
	Program: Parallel program for Matrix-Vector Multiplication using MPI

	Done by: Eng. Meqdad Darweesh

*/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 40 // Number of Rows and Columns of Entered Matrix


main(int argc, char **argv) {
	// ............:::::::: Variables and Matrices Declaration Start ::::::::............

		int my_rank, p, i, j;
		/*
			* my_rank: for ranking the processors
			* p: represent the number of processors in execution
			* i,j: for indexing in loops
		*/
		
		double t0 = 0.0, t1 = 0.0, t2 = 0.0, t3 = 0.0, t4 = 0.0, t5 = 0.0, t6 = 0.0, t7 = 0.0, t10 = 0.0,
		t_tot, t_comm, t_comp, t_wait; // These variables for timing (not all used)

		t0 = MPI_Wtime(); // build-in function in MPI for reading the time

		MPI_Status status;

		MPI_Init(&argc, &argv);
		MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
		MPI_Comm_size(MPI_COMM_WORLD, &p);

		long local_size, x;
	
		local_size = N / p; // This line for calculating the local size of M for each process, for distribution
				
		t1 = MPI_Wtime();

	// ............:::::::: Variables and Matrices Declaration End ::::::::............


// ............:::::::: Master Part Start ::::::::............

	if (my_rank == 0) {

	// ............:::::::: Master Declarations Start ::::::::............

		t2 += MPI_Wtime();

		long **M;
		long *R;
		long *V = (long *)malloc(N * sizeof(long));

		/*Reserve Mem. for 2-D array*/
		M = (long **)malloc(N * sizeof(long*));

		for (i = 0;i<N;i++)
			M[i] = (long *)malloc(N * sizeof(long));

		R = (long *)malloc(N * sizeof(long));

		int indexRow;

		t3 += MPI_Wtime();

	// ............:::::::: Master Declarations End ::::::::............


	// ............:::::::: Master Initialization Start ::::::::............ 

		t2 += MPI_Wtime();

		for (i = 0; i < N;i++) {
			for (j = 0;j < N;j++) {
				M[i][j] = (rand() % 10 + 1); // Initializing M matrix elements with Random numbers.
			}
		}

		printf("\nMatrix M initialization finished on Master\n");

		for (i = 0; i<N; i++)
			V[i] = (rand() % 10 + 1);

		printf("Vector initialization finished on Master\n");

		t3 += MPI_Wtime();

		 // Calucalting Initialization time.

	// ............:::::::: Master Initialization End ::::::::............ 

	// ............:::::::: Distributing Data Start ::::::::............

		t6 += MPI_Wtime();

		for (i = 1; i<p; i++) {
			MPI_Send(&(V[0]), N, MPI_LONG, i, 0, MPI_COMM_WORLD);

			indexRow = i*local_size;
			MPI_Send(&(M[indexRow][0]), local_size*N, MPI_LONG, i, 1, MPI_COMM_WORLD);
		}
		printf("Master sent vector V to all workers\n");
		printf("Master sent matrix M parts for all workers\n");

		t7 += MPI_Wtime();

	// ............:::::::: Distributing Data End ::::::::............


	// ............:::::::: Multiplication on Master Start ::::::::............
		t4 += MPI_Wtime();
		for (i = 0;i < local_size;i++) {
			R[i] = 0;
			for (j = 0;j < N;j++)
				R[i] += M[i][j] * V[j] ;
		}
		t5 += MPI_Wtime();

	// ............:::::::: Multiplication on Master End ::::::::............


	// ............:::::::: Receiving vector R Start ::::::::............
		t8 += MPI_Wtime();
		// Printing a part from the vector R which is calculated by Master
		for (i = 0;i < local_size;i++) {
			x = R[i];
			printf("R[ %d ] \n", x);
		}

		// Receiving the matrix after multiplication from workers
		int k;

		t9 += MPI_Wtime();

		for (i = 1; i<p; i++) {

			indexRow = i*local_size;
			
			t6 += MPI_Wtime();

			MPI_Recv(&(R[indexRow]), local_size, MPI_LONG, i, 2, MPI_COMM_WORLD, &status);
			
			t7 += MPI_Wtime();
			
			t8 += MPI_Wtime();
			for (k = 0;k < local_size;k++) {
				x = R[indexRow+k];
				printf("R[ %d ] \n", x);
			}
			t9 += MPI_Wtime();
		}

	// ............:::::::: Receiving vector R End ::::::::............
	
		t3 += MPI_Wtime();

		printf("Master received vector R from all workers\n");

		t10 += MPI_Wtime();

		t_tot = t10 - t0;
		tt = t9 - t8;
		t_init = t3 - t2 + (t1 - t0);
		t_comp = t4 - t5;
		t_comm = t7 - t6;
		t_init = t3 - t2;
		t_wait = t_tot - t_comm - t_comp - t_init - tt;

		printf("Total Time = %lf \n", t_tot); // done

		printf("Time spent in Computation = %lf \n", t_comp);
		
		printf("Time spent in Communication = %lf \n", t_comm);

		printf("Time spent in Initialization = %lf \n", t_init);

		printf("Time spent in Waiting = %lf \n", t_wait);

	}
// ............:::::::: Master Part End ::::::::............



// ............:::::::: Workers Part Start ::::::::............

	if (my_rank>0){

		t2 += MPI_Wtime();
	// ............:::::::: Allocating M_Worker & V Vector Start ::::::::............

		// Allocation Mem. for Vector V

		long *V = (long *)malloc(N * sizeof(long));

		// Allocation Mem. for M_Worker

		long **M_Worker = (long **)malloc(local_size*sizeof(long*));

		for (i = 0;i < local_size;i++)
			M_Worker[i] = (long *)malloc(N*sizeof(long));

		long *Rr = (long *)malloc(local_size*sizeof(long));

		t3 += MPI_Wtime();

	// ............:::::::: Allocating M_Worker & V Vector End ::::::::............


	// ............:::::::: Receiving Data Start ::::::::............

		t6 += MPI_Wtime();

		MPI_Recv(&(V[0]), N, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
		printf("Workers received vector V from Master\n");

		MPI_Recv(&(M_Worker[0][0]), (local_size*N), MPI_LONG, 0, 1, MPI_COMM_WORLD, &status);
		printf("Workers received matrix M parts from Master\n");

		t7 += MPI_Wtime();

	// ............:::::::: Receiving Data End ::::::::............


	//............:::::::: Multiplication of Matrix M_Worker Start ::::::::............
		t4 += MPI_Wtime();
		for (i = 0;i <local_size;i++) {
			Rr[i] = 0;
			for (j = 0;j<N;j++)
				Rr[i] += M_Worker[i][j] * V[j];
		}
		t5 += MPI_Wtime();
	//............:::::::: Multiplication of Matrix M_Worker End ::::::::............


	//............:::::::: Returning the Matrix distributed parts to Master Start ::::::::............

		// Sending the result matrix to master

		t6 += MPI_Wtime();

		MPI_Send(&(Rr[0]), local_size, MPI_LONG, 0, 2, MPI_COMM_WORLD);
		printf("Workers sent R vector parts to Master\n");

		t7 += MPI_Wtime();

	//............:::::::: Returning the Matrix distributed parts to Master End ::::::::............
	
	}

// ............:::::::: Workers Part Start ::::::::............


// ............:::::::: Finalizing and Releasing Start ::::::::............

	free(V);
	free(R);
	free(Rr);
    for(i=0; i<N; i++){
		free(M[i]);
		free(M_Worker[i]);
	}

	MPI_Finalize();

	exit(0);

// ............:::::::: Finalizing and Realising End ::::::::............

}
