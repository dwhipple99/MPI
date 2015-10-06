/* Time cost of message passing send/recv
* 
* Input: 
* Output: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mpi.h"
#define MAXMESSAGE 32*1024
#define NUMTIMINGS 100

double sbuffer[MAXMESSAGE];
double dbuffer[MAXMESSAGE];

main(int argc, char ** argv) 
 {
  int my_rank;
  /* My process rank */
  int p;
  /* Number of processes */
  int n;
  /* size of message */
  int start = 64;
  int step = 1024;
  int source;
  /* Process sending integral */
  int dest = 0;
  /* All messages go to 0 */
  int tag = 0;
  int i;
  double t0,t1;
  MPI_Status status;

  MPI_Init(&argc, &argv);


  MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
  MPI_Comm_size(MPI_COMM_WORLD,&p);
  printf("ready to go from %d\n",my_rank);

  if (p > 2) 
   {
    MPI_Finalize();
    exit(1);
   }

  source = 1;
  dest = 0;

  for (n=start;n<MAXMESSAGE;n = n+step) 
   {
    if (my_rank == 1) 
     {
      for (i=0;i<n;i++)
      sbuffer[i] = (double) i;
     }
    MPI_Barrier(MPI_COMM_WORLD);
    t0 = MPI_Wtime();
    for (i=0;i<NUMTIMINGS;i++) 
     {
      if (my_rank == 1) 
       {
        MPI_Send(sbuffer, n, MPI_DOUBLE, dest, tag, MPI_COMM_WORLD);
       }
      else 
       {
        MPI_Recv(dbuffer, n, MPI_DOUBLE, source, tag, MPI_COMM_WORLD, &status);
       }
     }
    if (my_rank == 0) 
     {
      t1 = MPI_Wtime();
      printf("time for message size %d = %f\n",n,(t1-t0)/NUMTIMINGS);
     }
   }

  MPI_Finalize();
 }
