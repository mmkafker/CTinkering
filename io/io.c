#include <stdio.h>

int main()
{

    FILE *read_file ;
    FILE *write_file; 

    read_file = fopen("read_test.cwr","rb");
    write_file = fopen("write_test.cwr","wb");
    int N; N = 18;
    double data[N]; 
    double tot;
    double temp[1]; 
    tot = 0.0; 
    *temp = 0.0;

    // Read N doubles from "read_file" to "data"
    fread(data , sizeof(double), N , read_file);
     
    int i;

    for (i=0;i<N;i++)
    {
	// Read one double at position i. "SEEK_SET" means from the beginning. 

	fseek(read_file,8*i,SEEK_SET); 	
	fread(temp,sizeof(double), 1 ,read_file);
	tot += (*temp);
    }

    printf("tot = %.17g\n",tot);
    *temp = tot;
    fwrite(temp,sizeof(double),1,write_file);
    
    fclose(read_file);
    fclose(write_file);
    return 0;
}  
