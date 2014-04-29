#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/utsname.h>    //uname
#include <sys/times.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <dirent.h>
#include <sys/stat.h>
#include <utime.h>
#include <fcntl.h>

#define MAX 100

char cmdline[MAX];
char* args[MAX];
char check[MAX][MAX] = {"cat", "cd", "cp", "date", "head", "ls", "ll", "ln", "ld", "mkdir", "mv", "pwd", "rm", "rmdir", "tail", "time", "touch", "uname"};
 
 
 /*---------------------- `cat` command ------------------------*/
int mycat(int argc, char** argv)
{
  char end;
  char wr = '>';
  char ovwr[] = ">>";
  char buf[MAX];
  char* f1, f2;
  int fp1, fp2;
  int nb=0;

	//echo to command line directly
	if(argc == 1)
	{
		
		printf("Type 'end' to terminate the program\n");
		while(strcmp(buf, "end") != 0)
		{
			gets(buf);
			puts(buf);
			
		}
	}
	
	else if(argc == 2)
	{
		//file already exists
		if(access(argv[1],F_OK|R_OK) == 0)
		{
			fp1 = open(argv[1], O_RDONLY);
			//file opened successfully
			if(fp1 != -1)
			{
				lseek(fp1, SEEK_SET, 0);
				//read successfully
				while((nb = read(fp1, buf, MAX)) > 0)
				{
					//write to output terminal succesfully
					write(1, buf, nb);
					
				}	
			}
			else
				perror("File error");
		}
		else
			perror("Access error");
	
	}
	
	//read from terminal and write to file
	else if(argc == 3)
	{
		//overwrite option used
		if(strcmp(argv[1],"-o") == 0)
		{
			//O_TRUNC removes existing file content, O_WRONLY opens file for writing purpose only
			fp1 = open(argv[2], O_TRUNC  |O_WRONLY);
			if(fp1 != -1)
			{
				//read successfully
				while((nb = read(0, buf, MAX)) > 0)
				{
					//write to output terminal succesfully
					write(fp1, buf, nb);
					
				}
				close(fp1);	
				
			
			}
		}
		
		else if(strcmp(argv[1], "-w") == 0)
		{
			//O_APPEND appends to existing file content, O_WRONLY opens file for writing purpose only
			fp1 = open(argv[2], O_WRONLY | O_APPEND);
			if(fp1 != -1)
			{
				//read from terminal
				while((nb = read(0, buf, MAX)) > 0)
				{
					write(fp1, buf, nb);
				}
				close(fp1);
			}
		}
					
		
				
	}
	
	//read from file 1 and write to file 2
	else if(argc == 4)
	{
		//3rd argument is the option and file 1 exists
		if((strcmp(argv[2], "-o") == 0) && (access(argv[1], F_OK|R_OK) == 0))
		{
			fp1 = open(argv[1], O_RDONLY);
			
			//if file 2 exists, truncate and open else create file 2 and open
			fp2 = open(argv[3], O_CREAT | O_TRUNC | O_WRONLY);
			if(fp2 != -1)
			{
				//read from file 1 successfully
				while((nb = read(fp1, buf, MAX)) > 0)
				{
					//write to file 2 successfully
					write(fp2, buf, nb);
				}
				close(fp1);
				close(fp2);
				return 0;
			}
		}
	}
	
	else
	{
		printf("Incorrect usage:\n<command> | <command> <filename> | <command> -o <filename> | <command -w <filename> | <command> <filename_1> -o <filename_2>\n");
		return 0;
	}	
		
	return 0;
}
/*---------------------- end of `cat` command ------------------------*/

/*-------------------------- `cd` command -----------------------*/
int mycd(int argc, char** argv)
{
	//valid number of arguments
	if(argc==2)
	{
		//chdir executed successfully
		if(chdir(argv[1])==0)
		{
			printf("Directory changed to %s", argv[1]);
			system("pwd");
			printf("\n");
		}
		
		//error
		else
		{
			perror("Directory change error");
			return -1;
		}
		
	}
	
	//invalid number of arguments
	else
	{
		printf("Incorrect number of arguments : <command> <dir_name>\n");
		return -1;
	}
	
	return 0;
}
/*---------------------- end of `cd` command ------------------*/

/*------------------ `cp` command ------------------*/
int mycp(int argc, char** argv)
{
  char* buf[MAX];
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
/*---------------------- end of `cp` command ------------------*/					

/*---------------------- `date` command ------------------*/				
int mydate(int argc, char** argv)
{
  	const time_t* timv;
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

/*---------------------- end of `date` command ------------------*/

/*---------------------- `head` command ------------------*/
int myhead(int argc, char** argv)
{
  FILE* fp;
	int fd, n, count=0;
	char line [MAX];
	
	//number of lines to be printed are specified
	if(argc == 3)
	{
		n = atoi(argv[1]);
		fp = fopen(argv[2], "r");
	}
	
	//number of lines not specified, take 10 by default
	else if(argc == 2)
	{
		n = 10;
		fp = fopen(argv[1], "r");
	}
	
	else
	{
		printf("Incorrect usage : <command> <filename> | <command> <num_lines> <filename>\n");
		return -1;
	}
	
	if(fp == NULL)
	{
		perror("File error");
		return -2;
	}
		
	//print till counter variable 'count' == n
    while(fgets(line, sizeof line ,fp)!= NULL)
    {
  		if(count == n)
  			break;
  		fprintf(stdout,"%s",line); //print the file contents on stdout
      count++;		   
    }
     	fclose(fp);
      return 0;	
}
/*---------------------- end of `head` command ------------------*/			

/*---------------------- `ls` command ------------------*/
/* ls option to list all the files in the current directory */
int myls(int argc, char** argv) 
{
	char choice[] = "- i";
	struct dirent **list;
	int i,j,n;
	int check = 0;
	
	if(argc > 3)
	{
		printf("Incorrect usage : <command> | <command> <dirname>\n");
		return -1;
	}
	
	//if only command is given, assume current directory to be listed
	if(argc == 1)
	{
		argv[1] = ".";
	}
	
	if(argc == 2 && (strcmp(argv[1], choice)==0))
		check = 1;
	
	
	//argv[1] - pathname of directory
	//&list - pointer address of the sorted entries list
	//0 indicates all values are selected to be sorted
	//4th argument is the compar argument, if NULL array is unsorted
	//alphasort - takes 2 arguments at a time and sorts them within the array
	//returns number of entries in the array
	if((n=scandir(argv[1], &list, 0, alphasort)) != -1)
	{
		//files sorted successfully
		for (i = 0; i < n; i++) 
		{
			//filename is printed in row and column format
			if(check)
				printf("%ld\t", list[i]->d_ino);
			printf("%s\n", list[i]->d_name);
			
			free(list[i]);
		}
		
		printf("Number of files : %d\n", n);
		free(list);
  }
  return 0; 
        
}
/*---------------------- end of `ls` command ------------------*/
/*
struct dirent { 
             ino_t      d_ino;                // file number of entry 
             __uint16_t d_reclen;             // length of this record 
             __uint8_t  d_type;               // file type, see below 
             __uint8_t  d_namlen;             // length of string in d_name 
             char    d_name[255 + 1];   // name must be no longer than this 
     } */
/* #define DT_UNKNOWN       0
     #define DT_FIFO          1
     #define DT_CHR           2
     #define DT_DIR           4
     #define DT_BLK           6
     #define DT_REG           8
     #define DT_LNK          10
     #define DT_SOCK         12
     #define DT_WHT          14
  */
  
//  struct stat { 
  //       dev_t    st_dev;    /* device inode resides on */
  //       ino_t    st_ino;    /* inode's number */
  //       mode_t   st_mode;   /* inode protection mode */
  //       nlink_t  st_nlink;  /* number of hard links to the file */
  //       uid_t    st_uid;    /* user-id of owner */
  //       gid_t    st_gid;    /* group-id of owner */
  //       dev_t    st_rdev;   /* device type, for special file inode */
  //       struct timespec st_atimespec;  /* time of last access */
  //       struct timespec st_mtimespec;  /* time of last data modification */
  //       struct timespec st_ctimespec;  /* time of last file status change */
  //       off_t    st_size;   /* file size, in bytes */
  //       quad_t   st_blocks; /* blocks allocated for file */
  //       u_long   st_blksize;/* optimal file sys I/O ops blocksize */
  //       u_long   st_flags;  /* user defined flags for file */
  //       u_long   st_gen;    /* file generation number */
  //   };
   
/*---------------------- `ll` command ------------------*/
/* ls -l i.e. long listing of files in a directory */
int myll(int argc, char** argv)
{
  struct stat buf;
  struct dirent* b1;
  DIR* dp;

	if (argc != 2)
	{
	    printf("Incorrect usage: <command> <pathname>\n");
	    return -1;
  	}
        
        dp = opendir(argv[1]);
        
        //if directory does not exist
        if(dp == NULL)
        {
        	printf("Error\n");
		      return -1;
        }
        
        printf("Type\tMode\tLink\tUID\tGID\tSize\tName\t\t\tTime\n");
        
        //read all the files present in the directory and print their attributes
	while((b1 = readdir(dp)) != NULL)
	{
		//load the buffer buf with attributes of file b1->d_name loaded in struct dirent
		stat(b1->d_name, &buf);
		switch (buf.st_mode & S_IFMT) 
		{
			case S_IFBLK:  printf("b");	//block device file      
					break;
			case S_IFCHR:  printf("c");	//character dev file
					break;
			case S_IFDIR:  printf("d");	//directory file        
					break;
			case S_IFIFO:  printf("p");	//FIFO/pipe file        
					break;
			case S_IFLNK:  printf("l");	//symbolic link file        
					break; 
			case S_IFREG:  printf("f");	//regular file        
					break;
			case S_IFSOCK: printf("s");	//socket        
					break;
			default:       printf("?");	//unknown        
					break;
        	}
        	
		printf("\t%lo\t", (unsigned long) buf.st_mode);			    //print mode in octal
		printf("%ld\t", (long) buf.st_nlink);				            //print hard link count
		printf("%ld\t%ld\t",(long) buf.st_uid, (long) buf.st_gid);	//print UID & GID of file
		printf("%lld\t", (long long) buf.st_size);			        //print size of the file in bytes
		//printf("%ld\t", (long) buf.st_ino);				            //print inode number of the file
		printf("%-20s\t", b1->d_name);					                //print name of the file
		printf("%s", (char *) (ctime(&buf.st_atime)));			    //print last access time of the file in string
		                                                        //format         
	}
	closedir(dp);
	return 0;
}
	
/*---------------------- end of `ll` command ------------------*/

/*---------------------- `ln` command ------------------*/
int myln(int argc, char* argv[])
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
	
/*---------------------- end of `ln` command ------------------*/

/*---------------------- `ld` command ------------------*/
/* ls command for checking if multiple files/directories exists or not */
int myld(int argc, char** argv)
{
	int i = 1;
	while(argv[i]!=NULL)
	{
		if(access(argv[i], F_OK) == 0)
		{
			printf("%s\n", argv[i]);
			i++;
		}
		else
		{
			printf("%s: ls: No such file or directory\n",argv[i]);
			i++;
		}
	}
	return 1;
}
/*---------------------- end of `ld` command ------------------*/

/*---------------------- `mkdir` command ------------------*/
int mymkdir(int argc, char** argv)
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

/*---------------------- end of `mkdir` command ------------------*/


/*---------------------- `mv` command ------------------*/
int mymv(int argc, char** argv)
{
	char* pathname;
	//3 arguments in command line
	if(argc==3)
	{	
		//check if file to be moved exists
		if((access(argv[1],F_OK|W_OK))==0)
		{
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
/*---------------------- end of `mv` command ------------------*/

/*---------------------- `pwd` command ------------------*/
int mypwd(int argc, char** argv)
{
	char value[256];
	//valid number of arguments
	if(argc == 1)
	{
		//check for PWD in environment list
		if(getcwd(value, sizeof(value))!=NULL)       //check if value exists in the environment list
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
/*---------------------- end of `pwd` command ------------------*/

/*---------------------- `rm` command ------------------*/
int myrm(int argc, char** argv)
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
/*---------------------- end of `rm` command ------------------*/	

/*---------------------- `rmdir` command ------------------*/
int myrmdir(int argc, char** argv)
{
	//Correct number of arguments
	if(argc==2)
	{
		//if deleted
		if(rmdir(argv[1]) == 0)
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
/*---------------------- end of `rmdir` command ------------------*/

/*---------------------- `tail` command ------------------*/
int mytail(int argc, char** argv)
{
	int fd, n, i, count=0;
	char line [MAX];
	FILE* fp;
  char lines[MAX][MAX];
	
	//number of lines specified
	if(argc == 3)
	{
		n = atoi(argv[1]);
		fp = fopen(argv[2], "r");
	}
	
	//number of lines not specified, taken as last 10 lines by default
	else if(argc == 2)
	{
		n = 10;
		fp = fopen(argv[1], "r");
	}
	
	//incorrect number of arguments
	else
	{
		printf("Incorrect usage : <command> <filename> | <command> <num_lines> <filename>\n");
		return -1;
	}
	
	if(fp == NULL)
	{
		perror("File error");
		return -2;
	}
	
	//read the entire file as lines and store 'line' in 2d array 'lines'
    	while(fgets(line, sizeof line ,fp)!= NULL)
    	{
    		strcpy(lines[count], line);
    		count++;
    	}	
     	fclose(fp);
     	
     	//count is the total number of lines in file
     	//n is number of lines to be printed
     	for(i=(count - n); i<=count; i++)
     	{
     		printf("%s", lines[i]);
     	}
     	
      	return 0;	
}
/*---------------------- end of `tail` command ------------------*/

/*---------------------- `time` command ------------------*/      
int mytime(int argc, char** argv)
{
  struct tms buf;
	//Invalid number of arguments
	if(argc != 1)
	{
		printf("Incorrect number of arguments : <command>\n");
		return -1;
	}
	
	//call the times() function and populate the buffer 'buf' with values
	if(times(&buf) != -1)
	{
		printf("User time : %ld\n",buf.tms_utime);
		printf("System time : %ld\n",buf.tms_stime);
		printf("User time of children : %ld\n",buf.tms_cutime);
		printf("System time of children : %ld\n",buf.tms_cstime);
		return 0;
	}
}
/*---------------------- end of `time` command ------------------*/	

/*---------------------- `touch` command ------------------*/
int mytouch(int argc, char** argv)
{
	//number of arguments = 2
	if(argc==2)
	{
		//if file exists
		if((access(argv[1],F_OK|W_OK))==0)
		{
			//if access time changed
			if(utime(argv[1],NULL)==0)    //NULL indicates set to the current time; only SU or owner of file can
			                              //define time to be set if other than current time
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
/*---------------------- end of `touch` command ------------------*/	

/*---------------------- `uname` command ------------------*/
int myuname(int argc, char** argv)
{
	struct utsname buf;
	if(argc > 2)
		return -1;
	
	
	if(uname(&buf) != -1)
	{
		//if user inputs only uname or uname -s
		if(argc == 1 || ((strcmp(argv[1], "-s") == 0) && argc == 2))
		{
			//print os name
			printf("OS Name : %s\n", buf.sysname);
			return 0;
		}
		
		//if user inputs uname -r
		if(strcmp(argv[1], "-r") == 0)
		{
			printf("Release: %s\n", buf.release);
			return 0;
		}
		
		//user inputs uname -v
		if(strcmp(argv[1], "-v") == 0)
		{
			printf("Version : %s\n", buf.version);
			return 0;
		}
	}
	return 0;
}
/*---------------------- end of `uname` command ------------------*/		

/*---------------------- `clear` command ------------------*/		
void myclear()
{
	/*
	we print the escape sequence that we get via running this command
		$ tput clear | od -c
		0000000 033   [   H 033   [   2   J
		
	\033 is octal for Esc and thus we print these sequences every time we call clear.. if clear command does not run successfully the output of the above command must be printed instead of the existing printf() statement
	*/
	printf("\033[H\033[2J");
} 

/*---------------------- end of `clear` command ------------------*/

int main(int argc, char** argv)
{
	int count=0, i=0, cmp, j, valid=0;
	char key[MAX];
	
	//run the shell till user inputs 'quit' or Ctrl-C
	while(strcmp(cmdline, "quit") != 0)
	{
		count = 0;
		
		//our shell's prompt
		printf("afreen@myShell $ ");
		//get command line input
		gets(cmdline);
		
		//tokenize the CL input delimited by " " using strtok() into args
		args[count++] = strtok(cmdline, " ");
		
		//tokenize the entire string till the next token encountered
		//does not have a NULL pointer
		while((args[count++] = strtok(NULL, " ")));
		count--;		//no of arguments (argc)
		
		//if user wants to clear screen
		if(strcmp(args[0], "clear") == 0)
		{
			myclear();
		}
		
		//checking for the input command
		strcpy(key, args[0]);
		for(i=0; i<18; i++)	//look through our check array for command
		{
		  if(strcmp(check[i], key) == 0)
		  {
		      valid = -1;
		      switch(i)
		      {
		        case 0:   mycat(count, args);
		                  break;
		        
		        case 1:   mycd(count, args);
		                  break;
          
		        case 2:   mycp(count, args);
		                  break;
		         
		        case 3:   mydate(count, args);
		                  break;
		          
		        case 4:   myhead(count, args);
		                  break;
		                  
		        case 5:   myls(count, args);
		                  break;
		                  
		        case 6:   myll(count, args);
		                  break;
		                  
		        case 7:   myln(count, args);
		                  break;
		                  
		        case 8:   myld(count, args);
		                  break;
		                  
		        case 9:   mymkdir(count, args);
		                  break;
		                  
		        case 10:  mymv(count, args);
		                  break;
		                  
		        case 11:  mypwd(count, args);
		                  break;
		                  
		        case 12:  myrm(count, args);
		                  break;
		                  
		        case 13:  myrmdir(count, args);
		                  break;
		                  
		        case 14:  mytail(count, args);
		                  break;
		                  
		        case 15:  mytime(count, args);
		                  break;
		                  
		        case 16:  mytouch(count, args);
		                  break;
		                  
		        case 17:  myuname(count, args);
		                  break;
		                  		        
		        default: printf("%s: Command not found\n", key);
		    		  break;
		      }	
		      break;  
		  }
		}
		
		
  	}
  	return 0;
}
