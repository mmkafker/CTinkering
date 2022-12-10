
#include <stdio.h>
 
int main()
{
    int N = 100000;
    int temp[N];
    long int tot; tot = 0;
    int i;
    for (i=0;i<N;i++) temp[i] = i;
    for (i=0;i<N;i++) tot +=temp[i];
    printf("tot = %ld\n",tot);

  
    return 0;
}
