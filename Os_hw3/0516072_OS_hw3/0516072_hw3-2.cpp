#include <bits/stdc++.h>
#include <algorithm>
#include<fstream>
#define min(a,b) ((a)<(b)?(a):(b))

using namespace std;

struct Process{
	int pid;
	int burst_t;
	int burst_count_down;
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
	int num;

	ifstream fin("Q2.txt");
	fin>> num;

	Process p[num+1];

	for(int i=1;i<=num;i++){
		fin >> p[i].arr_t;
	}
	for(int i=1;i<=num;i++){
		fin >> p[i].burst_t;
		p[i].burst_count_down=p[i].burst_t;
	}
	fin.close();

	for(int i=1;i<=num;i++){
		p[i].pid=i;
	}


	int flag;
	int total_t=0;
	int total_wt=0,total_tat=0;
	float avg_wt,avg_tat;
	for(int i=1;i<=num;i++){
		total_t+=p[i].burst_t;
	}
	int t=0;
	int cnt;
	int check[num+1]; ///check which process has arrived
	for(int i=1;i<=num;i++){
		check[i]=0;
	}
	while(t<total_t){
		cnt=0;
		for(int i=1;i<=num;i++){
			if(p[i].arr_t<=t && check[i]!= -1){
				cnt++;
				check[i]=1;
				//printf("i=%d\n",i);
			}
		}
		int m=100000000;
		if(cnt>0){
			//printf("cnt=%d\n",cnt);
			for(int i=1;i<=num;i++){
				if(check[i]==1 && p[i].burst_count_down>0){
					m= min(p[i].burst_count_down,m);
				}
			}
			flag=0;// only one process can be done once
			//printf("m=%d\n",m);
			for(int i=1;i<=num;i++){
				if(p[i].burst_count_down==m && p[i].burst_count_down>0){
					flag=1;
					t++;
					p[i].burst_count_down--;
					if(p[i].burst_count_down==0){
						cnt--;
						//printf("Process %d is Done!",i);
						p[i].end=t;
						//printf("%d\n",t);
					}
				}
			}
		}
		else{
			t++;
		}
		//printf("t=%d\n",t);
	}
	for(int i=1;i<=num;i++){
		//printf("p[%d].burst= %d\n", i,p[i].burst_t);
		p[i].wt	 = find_waiting_time(p[i].arr_t,p[i].burst_t,p[i].end);
		p[i].tat = find_turnaround_time(p[i].arr_t,p[i].end);
		//printf("p[%d].wt= %d\n", i,p[i].wt);
		total_tat+= p[i].tat;
		total_wt += p[i].wt;	
	}


	avg_tat	= (float)total_tat/(float)num;
	avg_wt	= (float)total_wt/(float)num;
	
	fstream file;

	file.open("ans2.txt",ios::out);
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