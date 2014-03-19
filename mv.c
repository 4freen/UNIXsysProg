#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char** argv)
{
	char* pathname;
	//3 arguments in command line
	if(argc==3)
	{	
		//check if file to be moved exists
		if((access(argv[1],F_OK|W_OK))==0)
		{
			//append name of the file to the given path name
			pathname = strcat(argv[2], argv[1]);
			//if link is succesfully created
			if(link(argv[1],argv[2])==0)
			{
				//if file argv[1] is deleted from its original location
				if((unlink(argv[1]))==0)
				{
					printf("%s succesfully moved to %s\n",argv[1],argv[2]);
					return 0;
				}
				else
				{
					//file argv[1] was not unlinked from original location
					perror("Link deletion error");
					return 99;
				}
			}
			//link creation failed
			else
			{
				perror("Link creation error");
				return 100;
			} 
		}
		
		//file argv[1] does not exist
		else
		{
			perror("File error");
			return 101;
		}
	}
	//invalid number of arguments
	else
	{
		printf("Invalid number of arguments : <command> <filename> <pathname/>\n");
		return 102;
	}
	
	return 0;
}
