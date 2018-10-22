#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include<sys/wait.h>

int main() {
    pid_t pid;
    int status=0;
    printf("Main process id : %d.\n",getpid());
    pid=fork();///fork1

    if(pid==0){
        printf("Fork1, I'm the child %d, my parent is %d.\n",getpid(),getppid());

        pid=fork(); ///fork2
        if(pid==0){
            printf("Fork2, I'm the child %d, my parent is %d.\n",getpid(),getppid());
        }
        else if(pid>0){
            wait(&status);
        }
        else{
            printf("Error!\n");
        }
    }
    else if(pid>0){
        wait(&status);
    }
    else{
        printf("Error!\n");
    }

    pid=fork(); ///fork3
    if(pid==0){
        printf("Fork3, I'm the child %d, my parent is %d.\n",getpid(),getppid());
    }
    else if(pid>0){
        wait(&status);
    }
    else{
        printf("Error!\n");
    }


    return 0;
}
