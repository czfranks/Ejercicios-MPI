#include <stdio.h>
#include <string.h>
#include "mpi.h"



int main()
{

  int my_rank;
  int num_proc;
  MPI_Init(NULL,NULL);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_proc);


  int rebotes = 0, max_rebotes = 5;
  int partner_rank = (my_rank + 1) % 2;
  while (rebotes < max_rebotes) {
      if (my_rank == rebotes % 2){
          // Increment the ping pong count before you send it
          rebotes++;
          MPI_Send(&rebotes, 1, MPI_INT, partner_rank, 0,
                   MPI_COMM_WORLD);
          printf("[%d] sent and incremented rebotes "
                 "%d to [%d]\n", my_rank, rebotes,
                 partner_rank);
      } else {
          MPI_Recv(&rebotes, 1, MPI_INT, partner_rank, 0,
                   MPI_COMM_WORLD, MPI_STATUS_IGNORE);
          printf("[%d] received rebotes %d from [%d]\n",
                 my_rank, rebotes, partner_rank);
      }
  }

  MPI_Finalize();

  return 0;
}

