#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUMBER_OF_THREADS 11 /* Hint */
#define PUZZLE_SIZE 9

/* example puzzle */
int puzzle[PUZZLE_SIZE+1][PUZZLE_SIZE+1] = {
			{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0},
			{-1,0,0,0,0,0,0,0,0,0}
		};

/* data structure for passing data to threads */
typedef struct
{
	int thread_number;
	int x;
	int y;
	int flag;
	int sum;
} parameters;

int row_sum,col_sum,grid_sum;

/* print puzzle */ 
#define LINE "====================== \n"
#define COLLINE "\n___________________________________ \n"
#define NAME "||  SUM   CHECKER  || \n"
void print_grid(int grid[10][10])
{
    int i,j;
    printf(LINE);
    printf(NAME);
    printf(LINE);

	for (i = 1; i < 10; i++)
	{
		for (j = 1; j < 10; j++)
		{
	        printf("|%1d |",grid[i][j]);
		}
        printf(COLLINE);
	}
    printf("\n");
}
// read file to check sudoku
void SetPuzzle(char filename[]){
    FILE *file = fopen(filename,"r");
    int i,j,ch,num;
    for (i=0;i<=9;i++){
        for(j=0;j<=9;j++){
            while(((ch = getc(file)) != EOF)){
                if(ch == EOF) break;
                if(ch == ',') break;
                if(ch == '\n') break;
                ungetc(ch,file);
                fscanf(file,"%d",&num);
                if(num!=-1)
                    puzzle[i][j] = num;            
            } 
        }
    }
    print_grid(puzzle);
    return;
}

void *check_row(void *arg){
	parameters *block=(parameters *)arg;
	int sum=block->sum;
	//sleep(100);
	int tmp_sum=0;
	for(int i=1;i<=PUZZLE_SIZE;i++){
		for(int j=1;j<=PUZZLE_SIZE;j++){
			tmp_sum+=puzzle[i][j];
		}
		//printf("%d\n",tmp_sum);
		if(tmp_sum!=sum){
			//printf("HAHAHA1\n");
			block->flag=0;
			pthread_exit(NULL);
		}
		tmp_sum=0;
	}
	pthread_exit(NULL);
}

void *check_col(void *arg){
	parameters *block=(parameters *)arg;
	int sum=block->sum;
	//sleep(100);
	//printf("%d\n",sum);
	int tmp_sum=0;
	for(int i=1;i<=PUZZLE_SIZE;i++){
		for(int j=1;j<=PUZZLE_SIZE;j++){
			tmp_sum+=puzzle[j][i];
		}
		if(tmp_sum!=sum){
			block->flag=0;
			//printf("HAHAHA2\n");
			pthread_exit(NULL);
		}
		tmp_sum=0;
	}
	pthread_exit(NULL);
}

void *check_grid(void *arg){
	parameters *block=(parameters *)arg;
	//sleep(100);
	int i=block->x;
	int j=block->y;
	int sum=block->sum;
	int tmp_sum=0;
	for(int m=i;m<i+3;m++){
		for(int n=j;n<j+3;n++){
			tmp_sum+=puzzle[m][n];
		}
	}
	if(tmp_sum!=sum){
		//printf("HAHAHA3\n");
		block->flag=0;
		pthread_exit(NULL);
	}
	pthread_exit(NULL);
}





int main(int argc, char* argv[])
{
	int rv = 1; // flag to check answer
    // input the sudoku file
    pthread_t thread_row,thread_col,thread_grid[10];

    for(int i=0;i<10;i++){
    	thread_grid[i]=0;
    }
    SetPuzzle("test2.txt");
    for(int i=1;i<=PUZZLE_SIZE;i++){
    	row_sum+=puzzle[1][i];
    }
    //printf("%d\n",row_sum);
    for(int i=1;i<=PUZZLE_SIZE;i++){
    	col_sum+=puzzle[i][1];
    }
    //printf("%d\n",col_sum);
    for(int i=1; i<=3;i++){
    	for(int j=1;j<=3;j++){
    		grid_sum+=puzzle[i][j];
    	}
    }
    //printf("%d\n",grid_sum);    
    parameters block;
    block.flag=1;

    block.sum=row_sum;
    pthread_create(&thread_row,NULL,check_row,(void*) &block);
    block.sum=col_sum;
    pthread_create(&thread_col,NULL,check_col,(void*) &block);
    int tmp=0;
    for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
			    block.x=i*3+1;
			    block.y=j*3+1;
			    block.thread_number=tmp;
			    block.sum=grid_sum;
			    tmp++;
			    pthread_create(&thread_grid[block.thread_number],NULL,check_grid,(void*) &block);
			    
    	}
    }

    tmp=0;
 	pthread_join(thread_row, NULL);
 	pthread_join(thread_col, NULL);

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            pthread_join(thread_grid[tmp],NULL);
            tmp++;
        }
    }

    rv=block.flag;


	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}
