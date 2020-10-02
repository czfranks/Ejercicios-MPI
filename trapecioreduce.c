
#include <stdio.h>

#include <mpi.h>

/* Calculate local integral  */
double Trap(double left_endpt, double right_endpt, int trap_count, 
   double base_len);    

/* Function we're integrating */
double f(double x); 

int main(int argc, char ** argv) {

   MPI_Init(&argc, &argv);

   //todos a la misma vez
 //  MPI_Barrier(MPI_COMM_WORLD );

   /*Preparando el calculo de tiempo transcurrido*/
   double local_start, local_finish, local_elapsed, elapsed;
   

   ///////////////////////////////////////////
   local_start = MPI_Wtime();
   ///////////////////////////////////////////

   int my_rank, comm_sz, n = 10*1000*1000, local_n;   
   double a = 0.0, b = 9.0, h, local_a, local_b;
   double local_int = 0.0, total_int = 0.0;

   /* Let the system do what it needs to start up MPI */
   
 
   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   h = (b-a)/n;          /* h  */
   local_n = n/comm_sz;  /* numero de trapecios para cada proceso */

   /* Rango de operacion para cada proceso*/
   local_a = a + my_rank * local_n * h;
   local_b = local_a + local_n * h;
   local_int = Trap(local_a, local_b, local_n, h);

   /*PTodos los procesos reducen(suman su valor) a el total_int
   */
   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM,/*proceso 0*/ 0, MPI_COMM_WORLD);
   /* Print the result */


   //////////////////////////////////////////
   local_finish = MPI_Wtime();
   //////////////////////////////////////////
   local_elapsed = local_finish - local_start;
   MPI_Reduce(&local_elapsed, &elapsed, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
   //////////////////////////////////////////



   if (my_rank == 0) {
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15f\n",
          a, b, (float)total_int);

      printf("Elaspsed time = %f seconds\n", (float)elapsed);
   }

   /* Shut down MPI */
   MPI_Finalize();

   return 0;
} /*  main  */


double Trap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {

   double estimate, x; 
   int i;

   estimate = (f(left_endpt) + f(right_endpt))/2.0;
   for (i = 1; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += f(x);
   }
   estimate = estimate*base_len;

   return estimate;

} /*  Trap  */



double f(double x) {
   return x*x;
} /* f */