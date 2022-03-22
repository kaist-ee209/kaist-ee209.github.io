/* circle.c */
#include <stdio.h>

double PI = 3.141593;  // or (4.0 * atan(1.0))

/* Calculate diameter and circumference of a circle
   from its radius given. */
int
main(void) {
    int radius = 5;
    int diameter;
    double circum;
    diameter = radius * 2;
    circum = PI * (double)diameter;

    // print results
    printf("A circle with radius %d has diameter %d\n",
    	   radius, diameter);
    printf("and circumference %f\n", circum);
    return 0;
}
