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

void *check(void *arg){
	parameters *block=(parameters *)arg;

	int i,j;
	//sleep(100);
	int tmp_sum_row=0;
	for(i=1;i<=PUZZLE_SIZE;i++){
		for(j=1;j<=PUZZLE_SIZE;j++){
			tmp_sum_row+=puzzle[i][j];
		}
		//printf("%d\n",tmp_sum);
		if(tmp_sum_row!=row_sum){
			//printf("HAHAHA1\n");
			block->flag=0;
			pthread_exit(NULL);
		}
		tmp_sum_row=0;
	}

	int tmp_sum_col=0;
	for( i=1;i<=PUZZLE_SIZE;i++){
		for( j=1;j<=PUZZLE_SIZE;j++){
			tmp_sum_col+=puzzle[j][i];
		}
		if(tmp_sum_col!=col_sum){
			block->flag=0;
			//printf("HAHAHA2\n");
			pthread_exit(NULL);
		}
		tmp_sum_col=0;
	}

	for(int i=0;i<3;i++){
    	for(int j=0;j<3;j++){
			    block->x=i*3+1;
			    block->y=j*3+1;	    
    	}
    }


	i=block->x;
	j=block->y;

	int tmp_sum_grid=0;
	for(int m=i;m<i+3;m++){
		for(int n=j;n<j+3;n++){
			tmp_sum_grid+=puzzle[m][n];
		}
	}
	if(tmp_sum_grid!=grid_sum){
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
    pthread_t t;


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


    pthread_create(&t,NULL,check,(void*) &block);


 	pthread_join(t, NULL);


    rv=block.flag;


	if (rv == 1)
		printf("Successful :) \n");
	else
		printf("Must check again! :( \n");

	return 0;
}
