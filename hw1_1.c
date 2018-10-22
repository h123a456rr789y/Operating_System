#include<stdlib.h>
#include<stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <dirent.h>
#include <sys/stat.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>
#include <fcntl.h>

#define MAX_LINE 80


int main(void)
{

	char *arg[MAX_LINE/2+1]; /*command line arguments*/
	int should_run = 1; /*flag to determine when to exit program*/
	char *input;
	int argcount =0,Background=0;


	while(should_run){
		printf("osh>");
		fflush(stdout);
		///////read line
		input=NULL; 
		ssize_t buf = 0;
    	getline(&input, &buf,stdin);
		////////////seperate of commands
		for(int i=0;i<MAX_LINE/2+1;i++){
			arg[i]=NULL;
		}
		argcount=0;
		Background=0;
		char *delim = " \n";
		char *w;
		w=strtok(input,delim);
		while(w!=NULL){
			arg[argcount]=w;
			argcount++;
			w=strtok(NULL,delim);
		}
		if(strcmp(arg[argcount-1],"&")==0){
			Background=1;
			arg[argcount-1]=NULL;
			argcount--;
		}
		///////functions
		if(strcmp(arg[0],"exit")==0){
			exit(1);
        }
		///////fork child process
		pid_t pid;

		pid=fork();
		if(pid==0){
			///child process
			//printf("%s",arg[0]);
			//execvp(arg[0],arg);
			if (execvp(arg[0],arg) < 0 ) {
                printf("ERROR: execution failed\n");
                exit(1);
            } 
		}
		else if(pid<0){
			printf("Failed to forking child\n");
		}
		else{
			/////parent
			if(Background==0){
				waitpid(pid, NULL, 0);
				//printf("Wait for child process.\n");
			}
			
		}
		if(strcmp(arg[0],"cd")==0){
        	chdir(arg[1]);
        }


		
		/**
		* your code!
		* After reading user input, the step are:
		* (1) fork a child process using fork()
		* (2) the child process will invoke execvp()
		* (3) if command included &, parent will invoke wait()
		*/

	}

	return 0;
}

