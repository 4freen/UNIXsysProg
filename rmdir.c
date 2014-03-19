#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char** argv)
{
	//Correct number of arguments
	if(argc==2)
	{
		//if deleted
		if(rmdir(argv[1])==0)
		{
			printf("Directory removed : %s\n",argv[1]);
			return 0;
		}
		
		//if not deleted
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
