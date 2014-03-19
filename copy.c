#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#define MAX 1000
char* buf[MAX];

int main(int argc, char** argv)
{
	int fd1, fd2;
	//valid number of arguments
	if(argc==3)
	{
		fd1=open(argv[1],O_RDONLY);
		//check if file is opened
		if(fd1==-1)
		{
			perror("File creation error");
			return 99;
		}
		//file created : file descriptor returned
		else
		{
			fd2=open(argv[2],O_WRONLY|O_CREAT,0777);
			//if file is created
			if(fd2!=-1)
			{
				if(read(fd1,buf,MAX)!=-1)
				{
					if(write(fd2,buf,MAX)!=-1)
					{
						printf("File %s copied to %s\n",argv[1],argv[2]);
						return 0;
					}
					
				}
				//file had read/write errors; hence delete the 2nd file created
				else
				{
					if(!unlink(argv[2]))
					{
						return 99;
					}
				}
			
			}
			
		}
	}
	//invalid number of arguments
	else
	{
		printf("Error: Enter command followed by file to be copied and destination file\n");
		return 100;
	}
}
					
				
			
			
		
