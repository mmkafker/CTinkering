#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

/* Function prototype */
double find_min(double* numbers, int length);

int main()
{

    FILE *x_file ; FILE *y_file; FILE *p_file; FILE *vx_file; FILE *vy_file;
    FILE *x_write_file; FILE *y_write_file; 

    x_file = fopen("x.bin","rb");
    y_file = fopen("y.bin","rb");
    p_file = fopen("particles.bin","rb");
    vx_file = fopen("vx.bin","rb");
    vy_file = fopen("vy.bin","rb");

    // Simulation Parameters
    int N; N = 5000;
    int numsteps = 10000;
    long int numvs = 5000*((long int) numsteps);
    double R = 1.0;
    double x[N];
    double y[N];
    int *particle; particle = malloc(numsteps*sizeof(int));
    double *vx; vx = malloc(numvs*sizeof(double));
    double *vy; vy = malloc(numvs*sizeof(double));
    double dt = 0.5;
    double bs = 150.0;

    fread(x , sizeof(double), N , x_file);
    fread(y , sizeof(double), N , y_file);
    fread(particle , sizeof(long int) , numsteps, p_file);
    fread(vx, sizeof(double) , numvs, vx_file);
    fread(vy, sizeof(double) , numvs, vy_file);
     

    // IO tests.
    //for (i=0;i<N;i++) printf("{%.17g , %.17g}\n",x[i],y[i]);
    //for (i=0;i<numsteps;i++) printf("%d, ", particle[i]);
    //double tot; tot = 0.0;
    //for (i=0;i<numvs;i++) tot+=vx[i]+vy[i];
    //printf("tot = %.17g\n",tot);
    
    int  p, j; long int vindex;long int i;
    int numtrials = 0; int maxtrials = 5000;
    int numrejects = 0;
    i = 0; vindex = 0; p = 0;
    double xtemp, ytemp, xtrial, ytrial; 
    xtemp = 0.0; ytemp = 0.0; xtrial = 0.0; ytrial = 0.0;
    double sqdists[N-1]; for (i=0;i<N-1;i++) sqdists[i] = 1e15;
    int accept;
    double min; min = 0.0;
    
    // Begin Monte Carlo integration
    ////////////////////////////////
    i=0;
    while(i<numsteps)
    {
    if (i%1000==0) printf("\ni = %ld\n",i);
    accept = 0;
    p = particle[i];
    //printf("(x[%d],y[%d]) = (%.17g,%.17g)\n",p,p,x[p],y[p]);
    numtrials = 0;
    while(accept == 0 && numtrials<maxtrials)
    {
	if (vindex<numvs)
        {
	    xtrial = x[p] + dt*vx[vindex];
	    ytrial = y[p] + dt*vy[vindex];
	}
	else assert(1 == 2);        
	for(j=0;j<N;j++)
	{
            if(j!=p)
	    {
		xtemp = xtrial-x[j];ytemp = ytrial -y[j];
		if (xtemp > 0.5*bs) xtemp-=bs;
		if (xtemp < -0.5*bs) xtemp+=bs;

		if (ytemp > 0.5*bs) ytemp-=bs;
		if (ytemp < -0.5*bs) ytemp+=bs;

		sqdists[j] = pow(xtemp,2) + pow(ytemp,2);
                //printf("dist to particle %d is %.17g\n",j,sqdists[j]);
	    }
	}

        min = find_min(sqdists,N-1);


        //printf("min = %.17g\n",min);
	if (min > 4*R*R)
	{
            accept = 1;
            x[p] = xtrial; y[p] = ytrial;
	    if (x[p] < -0.5*bs) x[p] += bs;
	    if (y[p] < -0.5*bs) y[p] += bs;
	    if (x[p] > 0.5*bs) x[p] -= bs;
	    if (y[p] > 0.5*bs) y[p] -= bs;
	}
	else vindex+=1;
	numtrials+=1;
	if (numtrials >= maxtrials) numrejects+=1;
    }
    //printf("tot = %.17g\n",tot);
    //*temp = tot;
    //fwrite(temp,sizeof(double),1,write_file);
    
    //printf("(x[%d],y[%d]) = (%.17g,%.17g)\n",p,p,x[p],y[p]);
    i+=1;
    }
    printf("After loop, vindex = %ld and numrejects = %d\n",vindex,numrejects); 

    x_write_file = fopen("xnew.bin","wb");
    y_write_file = fopen("ynew.bin","wb");
    fwrite(x,sizeof(double),N,x_write_file);
    fwrite(y,sizeof(double),N,y_write_file);
    fclose(x_file);fclose(y_file); fclose(p_file); fclose(vx_file);fclose(vy_file);
    fclose(x_write_file);fclose(y_write_file);

    free(vx); free(vy); free(particle);
    //fclose(write_file);
    return 0;
}

/* Function definition */
double find_min(double* numbers, int length) {
    /* Initialize the minimum value to the first element in the array */
    double min = numbers[0];
    //printf("    Min function: j = 0, min = %.17g\n",min);
    /* Iterate over the rest of the array and update the minimum value if necessary */
    for (int j = 1; j < length; j++) 
    {
	if (numbers[j] < min) min = numbers[j];
        //printf("    Min function: j = %d, min = %.17g\n",j,min);
    }

    /* Return the minimum value */
    return min;
}
