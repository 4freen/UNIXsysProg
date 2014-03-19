#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<utime.h>
#include<fcntl.h>
#include<errno.h>

int main(int argc, char** argv)
{
	//number of arguments = 2
	if(argc==2)
	{
		//if file exists
		if((access(argv[1],F_OK|W_OK))==0)
		{
			//if access time changed
			if(utime(argv[1],NULL)==0)
			{
				return 0;
			}
			
			//access time not changed
			else
				perror("Error");
		}
		
		//file does not exist
		else
		{
			//file is created
			if(open(argv[1],O_CREAT|O_RDWR,0777)!=-1)
			{
				printf("File created : %s\n",argv[1]);
			}
			
			//file not created error
			else
				perror("File error");
		}
	}
	
	//incorrect number of arguments
	else
	{
		printf("Incorrect number of arguments : <command> <pathname>\n");
		return 100;
	}
}		
