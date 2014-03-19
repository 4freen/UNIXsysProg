#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>

int main(int argc, char** argv)
{
	
	if(argc==2)
	{	
		//if file at given pathname exists
		if(access(argv[1],F_OK|W_OK)==0)
		{
			//unlink API successful
			if(unlink(argv[1])==0)
			{
				printf("%s deleted\n",argv[1]);
			}
			
			//unlink API unsuccessful i.e. file not deleted
			else
			{
				perror("File deletion error");
				return 99;
			}
		}
		
		//given file at pathname does not exist
		else
		{
			perror("File access error");
			return 100;
		}
	}
	
	//invalid number of arguments
	else
	{
		printf("Incorrect number of arguments : <command> <file-pathname\n");
		return 101;
	}
}
