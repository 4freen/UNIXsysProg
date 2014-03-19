#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

int main(int argc, char* argv[])
{
	char* sym="-s";
	
	// creating a hard link
	if(argc==3)
	{
		//linking file argv[1] to argv[2]
		if((link(argv[1],argv[2]))==0)	
		{
			printf("Link for %s successfully created : %s\n",argv[1],argv[2]);
			return 0;
		}
		//error in creating hard link
		else
		{
			perror("Link error");
			return 100;
		}
	}
	//creating a symbolic link
	else if(argc==4)
	{
		//if option -s is used in command line
		if((strcmp(sym,argv[1]))==0)
		{
			//creating a symbolic link of file argv[2] as argv[3]
			if((symlink(argv[2],argv[3]))==0)
			{
				printf("Symbolic link for file %s successfully created : %s\n",argv[2],argv[3]);
				return 0;
			}
			else
			{
				perror("Symbolic link error");
				return errno;
			}
		}
		//error if option -s is not used
		else
		{
			printf("Invalid option used\n");
			return 100;
		}
	}
	//user input error
	else
	{
		printf("Incorrect number of arguments : <command> <filename> <linkname> OR <command> -s <filename> <linkname>\n");
		return 101;
	}
	return 0;
}
	
