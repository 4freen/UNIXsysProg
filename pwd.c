#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(int argc, char** argv)
{
	char* value;
	//valid number of arguments
	if(argc == 1)
	{
		//check for PWD in environment list
		if((value=getenv("PWD"))!=NULL)
		{
			printf("Current working directory : %s\n",value);
			return 0;
		}
		
		//failed to retrieve PWD from environment list
		else
		{
			printf("Current working directory not found\n");
			return -1;
		}
	}
	
	else
	{
		printf("Incorrect number of arguments : <command>\n");
	 	return -1;
	}
}
	
				
