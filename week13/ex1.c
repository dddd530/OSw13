#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int main(int argc, char *argv[])
{
	char *input_file = " ";
  char *output_dl = "output_dl.txt";
  char *output_ok = "output_ok.txt";

  if(argc == 1){
    input_file = argv[0];
  }
  if(argc == 2){
    input_file = argv[1];
  }
  if(argc > 2){
    printf("Incorrect command");
  }
	FILE *input = fopen(input_file, "r");
	char buffer[2000];
	
	int processes[200], resources_exist[200], resources_av[200],allocation[200][200], request[200][200];
	
	// reading line 1
	fgets(buffer, 2000, input);
	char *p = buffer;
	int e_count = 0;
	while(*p != '\n')
	{
		int temp = strtol(p, &p, 10);
		resources_exist[e_count++] = temp;
	}
	// reading line 2
	fgets(buffer, 2000, input);
	p = buffer;
	int av_count = 0;
	while(*p != '\n')
	{
		int temp = strtol(p, &p, 10);
		resources_av[av_count++] = temp;
	}
	// reading allocation matrix
	int proc_count = 0;
  int temp;
	while(strcmp(fgets(buffer, 2000, input), "\n") != 0)
	{
		p = buffer;
		for(int i = 0 ; i < e_count; i++)
		{
			temp = strtol(p, &p, 10);
			allocation[proc_count][i] = temp;
		}
		proc_count++;
	}
	
	// reading request matrix
	for(int i = 0; i < proc_count; i++)
	{
		fgets(buffer, 2000, input);
		p = buffer;
		for(int j = 0; j < e_count; j++)
		{
			temp = strtol(p, &p, 10);
			request[i][j] = temp;
		}
	}
	
	//processing data
	
  
	int processes_ok[200] = {};	

	
  //detecting deadlocks
  int true = 1;
	while(true)
    {
        int c = 0;
        for(int i = 0; i < proc_count; i++)
        {
            if(processes_ok[i])
                continue;
            int j = 0;
            for(j = 0; j < av_count; j++)
            {
                if(request[i][j] > resources_av[j])
                {
                    break;
                }
            }
            if(j != av_count)	
                continue;				
            for(j = 0; j < av_count; j++)
            {
                resources_av[j] += request[i][j] + allocation[i][j];
            }
            c = 1;	
            processes_ok[i] = 1;
        }
        if(c == 0)
            break;
    }
	

	//looking for deadlocks
	int deadlock = 0;
	for(int i = 0; i < proc_count; i++)
	{
		if(!processes_ok[i])
		{
			deadlock = 1;
			break;
		}
	}
	

  //placing output in corresponding file
	if(deadlock)
	{
    FILE *output = fopen(output_dl, "w");
		fprintf(output, "Deadlock between processes: ");
		for(int i = 0; i < proc_count; i++)
		{
			if(!processes_ok[i])
			{
				fprintf(output, "%d ", i);
			}
    }
    fclose(output);
	}
	else
	{
    FILE *output = fopen(output_ok, "w");
		fprintf(output, "No deadlock was detected");
    fclose(output);
	}
	return 0;
}