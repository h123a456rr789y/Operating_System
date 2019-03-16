#include<bits/stdc++.h>
#include<algorithm>
#include<queue>
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

int cnt,flag=0;

int find_waiting_time(int arr, int burst,int t){
	return	(t-arr-burst);
}

int find_turnaround_time(int arr , int t){
	return	(t-arr);
}


int main(){
	int num,qt0,qt1;
	queue <int> q0,q1,q2;
	

	ifstream fin("Q4.txt");
	fin>> num;

	Process p[num+1];

	for(int i=1;i<=num;i++){
		fin >> p[i].arr_t;
	}
	for(int i=1;i<=num;i++){
		fin >> p[i].burst_t;
		p[i].burst_count_down=p[i].burst_t;
	}
	fin >> qt0 >> qt1;
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
	int qt0_tmp=qt0;
	int qt1_tmp=qt1;
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
				q0.push(i);
				check[i]=1;
			}
		}
		if(q0.empty()){
			break;
		}
		int first=q0.front();
		
		if(p[first].burst_count_down>0 && qt0_tmp>0){
			p[first].burst_count_down--;
			t++;	
			qt0_tmp--;
			//printf("P%d ",first);
		}
		//print_queue(q);
		else if(p[first].burst_count_down>0 && qt0_tmp==0){
			//cout<< "AA"<<endl;
			p[first].end=t;
			//printf("push to q1:%d\n",first);
			q1.push(first);
			if(total_t<t){ /////jump
				p[first].end=t-1;
				//printf("BB\n");
			}
			//printf("AAAP%d t:%d\n",first,t);
			qt0_tmp=qt0;
			q0.pop();
		}
		else{	///p[first].burst_t==0
			p[first].end=t;
			if(total_t<t){ /////since jump t++ we use total
				p[first].end=t-1;
				//printf("CC\n");
			}
			//printf("BBBP%d t:%d\n",first,t);
			qt0_tmp=qt0;
			q0.pop();
		}	
	}

	//printf("%d\n",t);

	while(t<=total_t){
		if(total_t==t){ /////jump out of the loop 
			t++;
		}
		for(int i=1;i<=num;i++){
			if(p[i].arr_t<=t && check[i]!=1){
				q1.push(i);
				check[i]=1;
			}
		}
		if(q1.empty()){
			break;
		}
		int first=q1.front();
		
		if(p[first].burst_count_down>0 && qt1_tmp>0){
			p[first].burst_count_down--;
			t++;	
			qt1_tmp--;
			//printf("P%d ",first);
		}
		//print_queue(q);
		else if(p[first].burst_count_down>0 && qt1_tmp==0){
			//cout<< "AA"<<endl;
			p[first].end=t;
			q2.push(first);
			//printf("push to q2:%d\n",first);
			if(total_t<t){ /////jump
				p[first].end=t-1;
				//printf("BB\n");
			}
			//printf("P%d t:%d\n",first,t);
			qt1_tmp=qt1;
			q1.pop();
			//return first;
		}
		else{	///p[first].burst_t==0
			//cout<< "BB"<<endl;
			p[first].end=t;
			if(total_t<t){ /////since jump t++ we use total
				p[first].end=t-1;
				//printf("CC\n");
			}
			//printf("P%d t:%d\n",first,t);
			qt1_tmp=qt1;
			q1.pop();
		}	
	}


	//printf("%d\n",t);

	for(int i=1;i<=num;i++){
		check[i]=0;
	}
	
	cnt=0;
	for(int i=1;i<=num;i++){
		if(p[i].burst_count_down>0){
			cnt++;
			check[i]=1;
			//printf("i=%d\n",i);
		}
	}
	while(t<total_t){
		//printf("cnt=%d\n",cnt );
		int m=100000000;
		if(cnt>0){
			//printf("cnt=%d\n",cnt);
			for(int i=1;i<=num;i++){
				if(check[i]==1){
					//printf("i=%d\nburst=%d\n",i,p[i].burst_count_down);
					m= min(p[i].burst_count_down,m);
				}
			}
			flag=0;// only one process can be done once
			//printf("m=%d\n",m);
			for(int i=1;i<=num;i++){
				if(p[i].burst_count_down==m && flag==0 && check[i]!=-1){
					flag=1;
					check[i]=-1;
					t+=m;
					//printf("Process %d is Done!",i);
					p[i].end=t;
				}
			}
		}
		else{
			t++;
		}
		cnt--;	
		///printf("t=%d\n",t);
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

	file.open("ans4.txt",ios::out);
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