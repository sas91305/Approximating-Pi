/*
*Seth Schaller
*Sequentially Approximating Pi
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int main(int argc, char **argv)
{
	srand(time(0));
	double x, y, approx, difference, trialnum, in = 0;
	printf("Enter number of trials: ");
	scanf(" %lf", &trialnum);

	for (int i = 0; i < trialnum; i++) {
		x = (2.0 * ((double)rand() / (double)RAND_MAX)) - 1.0; //Random x-coordinate
		y = (2.0 * ((double)rand() / (double)RAND_MAX)) - 1.0; //Random y-coordinate
		if (sqrt(pow(x, 2) + pow(y, 2)) <= 1.0) //Distance = sqrt(x^2 + y^2)
			in++;
	}

	approx = 4.0 * in / trialnum;
	difference = fabs(approx - M_PI) / M_PI;
	printf("Approximization: %lf\n", approx);
	printf("Difference of approximization: %.7lf\n", difference);
	return 0;
}
