// Create a list of numbers up to N. Sum 1/(n+1).

#include <stdio.h>
#include <stdlib.h>

int main()
{
    long int N = 100000000;
    long int *temp;
    temp = malloc(N*sizeof(long int));
    double tot; tot = 0.0;
    long int i;
    for (i=0;i<N;i++) temp[i] = i;
    for (i=0;i<N;i++) tot +=1.0/(temp[i]+1.0);
    printf("tot = %.17g\n",tot);
    free(temp);
    return 0;
}
