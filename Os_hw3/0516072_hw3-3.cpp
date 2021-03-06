#include<bits/stdc++.h>
#include<algorithm>
#include<queue>
#include<fstream>
#define min(a,b) ((a)<(b)?(a):(b))

using namespace std;
struct Process{
	int pid;
	int burst_t;
	int burst_count_down; ///for counting the remaining burst time 
	int arr_t;
	int wt;
	int tat;
	int end;
};


int find_waiting_time(int arr, int burst,int t){
	return	(t-arr-burst);
}

int find_turnaround_time(int arr , int t){
	return	(t-arr);
}



int main(){
	int num,qt;
	queue <int> q;
	
	ifstream fin("Q3.txt");
	fin>> num;

	Process p[num+1];

	for(int i=1;i<=num;i++){
		fin >> p[i].arr_t;
	}
	for(int i=1;i<=num;i++){
		fin >> p[i].burst_t;
		p[i].burst_count_down=p[i].burst_t;
	}
	fin >> qt;

	fin.close();

	for(int i=1;i<=num;i++){
		p[i].pid=i;
	}

	int total_t=0;
	int total_wt=0,total_tat=0;
	float avg_wt,avg_tat;
	for(int i=1;i<=num;i++){
		total_t+=p[i].burst_t;
	}
	int t=0;
	int qt_tmp=qt;
	int flag=0;
	int check[num+1]; ///check which process has arrived
	for(int i=1;i<=num;i++){
		check[i]=0;
	}

	while(t<=total_t){
		if(total_t==t){ /////jump out of the loop 
			t++;
		}
		for(int i=1;i<=num;i++){
			if(p[i].arr_t<=t && check[i]!=1){
				q.push(i);
				check[i]=1;
			}
		}
		int first=q.front();
		
		if(p[first].burst_count_down>0 && qt_tmp>0){
			p[first].burst_count_down--;
			t++;	
			qt_tmp--;
			//printf("P%d ",first);
		}
		//print_queue(q);
		else if(p[first].burst_count_down>0 && qt_tmp==0){
			//cout<< "AA"<<endl;
			q.push(first);
			p[first].end=t;
			if(total_t<t){ /////jump
				p[first].end=t-1;
				//printf("BB\n");
			}
			//printf("P%d t:%d\n",first,t);
			qt_tmp=qt;
			q.pop();
		}
		else{	///p[first].burst_t==0
			//cout<< "BB"<<endl;
			p[first].end=t;
			if(total_t<t){ /////since jump t++ we use total
				p[first].end=t-1;
				//printf("CC\n");
			}
			//printf("P%d t:%d\n",first,t);
			qt_tmp=qt;
			q.pop();

		}
		
	}




	for(int i=1;i<=num;i++){
		//printf("p[%d].end= %d\n", i,p[i].end);
		p[i].wt	 = find_waiting_time(p[i].arr_t,p[i].burst_t,p[i].end);
		p[i].tat = find_turnaround_time(p[i].arr_t,p[i].end);
		//printf("p[%d].wt= %d\n", i,p[i].wt);
		total_tat+= p[i].tat;
		total_wt += p[i].wt;	
	}


	avg_tat	= (float)total_tat/(float)num;
	avg_wt	= (float)total_wt/(float)num;
	
	fstream file;

	file.open("ans3.txt",ios::out);
	if(!file){
		cout<<"File can't be open\n";
	}
	else{
		for(int i=1;i<=num;i++){
			for(int j=1;j<=num;j++){
				if(p[j].pid==i){
					file <<p[j].wt<<" "<< p[j].tat<<endl;
				}
			}
		}
		file<<avg_wt<<endl;
		file<<avg_tat<<endl;
		file.close();
	}

	return 0;

}