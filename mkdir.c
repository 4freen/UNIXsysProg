#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char** argv)
{
	//Correct number of arguments
	if(argc==2)
	{
		//create directory
		if(mkdir(argv[1],0777)==0)
		{
			printf("Directory created : %s\n",argv[1]);
			return 0;
		}
		
		//directory not created
		else
		{
			perror("Directory error");
			return 1;
		}
	}
	//Invalid number of arguments
	else
	{
		printf("Incorrect number of arguments : <command> <dirname>\n");
		return 2;
	}
}
			
