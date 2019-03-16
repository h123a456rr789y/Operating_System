#include <bits/stdc++.h>
#include <algorithm>
#include<fstream>
#define min(a,b) ((a)<(b)?(a):(b))

using namespace std;
struct Process{
	int pid;
	int burst_t;
	int arr_t;
	int wt;
	int tat;
};

int find_waiting_time(int arr, int burst,int t){
	return	(t-arr-burst);
}

int find_turnaround_time(int arr , int t){
	return	(t-arr);
}

int main(){
	int num;
	ifstream fin("Q1.txt");
	fin>> num;

	Process p[num+1];

	for(int i=1;i<=num;i++){
		fin >> p[i].arr_t;
	}
	for(int i=1;i<=num;i++){
		fin >> p[i].burst_t;
	}
	fin.close();
	for(int i=1;i<=num;i++){
		p[i].pid=i;
	}
	
	/*
	for(int i=1;i<=num;i++){
		scanf("%d",&p[i].arr_t);
	}
	for(int i=1;i<=num;i++){
		scanf("%d",&p[i].burst_t);
	}*/
	
	int flag;
	int total_t=0;
	int total_wt=0,total_tat=0;
	int avg_wt,avg_tat;
	for(int i=1;i<=num;i++){
		total_t+=p[i].burst_t;
	}
	//printf("%d",total_t);

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
				if(check[i]==1){
					m= min(p[i].burst_t,m);
				}
			}
			flag=0;// only one process can be done once
			//printf("m=%d\n",m);
			for(int i=1;i<=num;i++){
				if(p[i].burst_t==m && flag==0 && check[i]!=-1){
					flag=1;
					check[i]=-1;
					t+=m;
					//printf("Process %d is Done!",i);
					p[i].wt	 = find_waiting_time(p[i].arr_t,p[i].burst_t,t);
					p[i].tat = find_turnaround_time(p[i].arr_t,t);

					total_tat+= p[i].tat;
					total_wt += p[i].wt;
				}
			}
		}
		else{
			t++;
		}
		cnt--;	
		///printf("t=%d\n",t);
	}

	//printf("%d\n",total_tat);
	//printf("%d\n",total_wt);
	/*for(int i=1;i<=num;i++){
		for(int j=1;j<=num;j++){
			if(p[j].pid==i){
				printf("%d %d\n",p[j].wt,p[j].tat);
			}
		}
	}*/

	avg_tat	= total_tat/num;
	avg_wt	= total_wt/num;
	
	//printf("%d\n%d\n",avg_wt,avg_tat);
	fstream file;
	file.open("ans1.txt",ios::out);
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