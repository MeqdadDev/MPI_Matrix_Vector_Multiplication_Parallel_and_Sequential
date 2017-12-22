/*

    Program: Matrix-Vector Multiplication (Sequential)

    Done by: Eng. Meqdad Darweesh

    This program used for comparison with parallel program
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>

#define N 40

//This function used to generate a random numbers for matrix vector initialization
long my_random(int min, int max) 
 {
 	int fd = open("/dev/urandom", O_RDONLY);
 	int nr;
 	read(fd, &nr, sizeof(nr));
 	close(fd);
 	return (long)abs(min + (double)(max - min) * nr / INT_MAX);
 }
 
// This function will read the system time from a file system which is located in the path:/proc/uptime 
 double get_uptime() 
{
  FILE* fp;
  double uptime, idle_time;
 
  fp = fopen ("/proc/uptime", "r");
  fscanf (fp, "%lf %lf\n", &uptime, &idle_time);
  fclose (fp);
  return(uptime);
}
  
  
int main(int argc, char* argv[])
{
    double t0,t1,t3,timeComp; // variables to store the time values
    int i,j; // Matrix - Vector indices 
    long **M,*V,*R; // M: square matrix..... V: vector..... R: vector to store the result.
    

    // Allocatation for M with long data type....
 	  M = (long **)malloc(N*sizeof(long));
    for (i = 0;i < N;i++)
		  M[i] = (long *)malloc(N * sizeof(long)); 

    // Allocatation for vector V with long data type....
	V = (long *)malloc(N * sizeof(long));
     
    // Allocatation for vector R with long data type....
	R = (long *)malloc(N * sizeof(long)); 
    
    // Initialization for matrix M with random numbers
    for (i = 0; i < N;i++){
		  for (j = 0;j < N;j++){
			  M[i][j] = my_random(0,10);
		  }
	  }

    printf("\nMatrix initialization finished\n");
    
    // Initialization for vector V with random numbers
    for(i=0; i<N; i++){
        V[i] = my_random(0,10);
    }

    printf("Vector initialization finished\n");



    // Matrix Vector Multiplication    
    t1=get_uptime(); // storing the current time before computation process.

    for (i = 0; i < N;i++){
		  for (j = 0;j < N;j++){
			  R[i] = V[j] * M[i][j];
		  }
	  }

    t3=get_uptime(); // storing the current time after computation process
    
    printf("Time Calculation....\n");

    timeComp = t3 - t1;
    
    printf("Time spent in computation = %f \n", timeComp);
    
    free(V);
    free(R);
    for(i=0; i<N; i++)
        free(M[i]);
    
    return 0;
}

