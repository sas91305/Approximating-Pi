/*
* Parallelized Approximization of Pi
* Author: Seth Schaller
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define PRNG_BUFSZ 32

int main(int argc, char **argv)
{
    double trialnum, approx, difference;
    int nthreads = omp_get_max_threads();
	
	//User specified nummber of trials
    printf("Enter number of trials: ");
    scanf(" %lf", &trialnum);
    
    //random_r(); thread-safe PRNGS - one per thread
	//---------------------------------------------------
    srandom( time(NULL) );
    char *rand_state_bufs = (char *) calloc(nthreads, PRNG_BUFSZ);
    struct random_data *rand_states = 
        (struct random_data *)calloc(nthreads, sizeof(struct random_data));
    
	//Initialize a PRNG for each thread
    for(int t = 0; t<nthreads; t++)
        initstate_r(random(), &rand_state_bufs[t], PRNG_BUFSZ, &rand_states[t]);
    //-----------------------------------------------------

	//The following block is repeated on each thread
#pragma omp parallel num_threads(nthreads)
    {
		//Variables local to each thread
        double in = 0.0;
        int threadID = omp_get_thread_num();

		//for loop divided up between threads
#pragma omp for
        for (int i = 0; i < (long)trialnum; i++) {
            int rnx, rny;
            random_r(rand_states + threadID, &rnx);
            random_r(rand_states + threadID, &rny);
            double x = (2.0 * ((double)rnx / (double)RAND_MAX)) - 1.0; //Random x-coordinate
            double y = (2.0 * ((double)rny / (double)RAND_MAX)) - 1.0; //Random y-coordinate
            if (sqrt(pow(x, 2) + pow(y, 2)) <= 1.0) //Distance = sqrt(x^2 + y^2)
                in++;
        }

		//Critical section: Mutal exclusion; only one thread can execute at a time
#pragma omp atomic
        approx += 4.0 * in / (double)trialnum;
        difference = fabs(approx - M_PI) / M_PI;
        printf("Thread %d: approx: %lf.7\n", threadID, approx);
    }   
	//end of parallel block

    printf("Approx: %lf\n", approx);
    printf("Difference: %lf\n", difference);
    return 0;
}
