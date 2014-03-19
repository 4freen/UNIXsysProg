#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<stdbool.h>

//store time in numerical format
const time_t* timv;

int main(int argc, char** argv)
{
	//get local date & time
	timv = time(0);
	while(true)
	{
		//get the current time in string format
		printf("%s",ctime(&timv));
		break;
	}
	return 0;
}
	
