#include<iostream>
#include<queue>
#include<algorithm>
using namespace std;

struct Process_Data
{
	int Num;
	int Pid;                              // denotes the Id of the process
	int arr_time;                         // denotes the arrival time of the process for execution
	int bur_time;                         // denotes burst time of the process
	int Priority;                         // denotes Priority of the process
	int end_time;                         // time of complete execution of the process Finish Time
	int rem_time;                         // denotes remaining Time for complete Execution of the process
	int idle_time;                        // denotes for how much time time the process is in waiting state
	int start_time;                       // denotes the starting Time of the process
	int Res_time;                         // denotes response time

};

struct Process_Data cur;
typedef struct Process_Data temp ;


//sorting in the ascending order of process ids
bool idsort(const temp& x , const temp& y)
{
	return x.Pid < y.Pid;
}


// Sorting on the basis of arrival time if that match then on Priority if Priority also  matches then on the basis of Process Id
bool arrivalsort( const temp& x ,const temp& y)
{
	if(x.arr_time < y.arr_time)
		return true;
	else if(x.arr_time > y.arr_time)
		return false;
	if(x.Priority < y.Priority)
		return true;
	else if(x.Priority > y.Priority)
		return false;
	if(x.Pid < y.Pid)
		return true;

	return false;
}

bool Numsort( const temp& x ,const temp& y)
{
	return x.Num < y.Num;
}


//Sorting on the basis of Priority if they are same then on the basis of PID
struct comp
{
	bool operator()(const temp& x ,const temp& y)
	{
		if( x.Priority > y.Priority )
			return true;
		else if( x.Priority < y.Priority )
			return false;
		if( x.Pid > y.Pid )
			return true;

		return false;
		
	}
	
};



int main()
{
	int i;
	vector< temp > input;
	vector<temp> input_copy;
	temp t1;
	int level1_q_process = 0;                       // for 1st level queue process
	int level2_q_process = 0;                       // for 2nd level queue process
	int arr_time;
	int bur_time;
	int Pid;
	int Priority;
	int n;
	int clock;
	int total_exection_time = 0;
	cout<<"enter the total number of processes : ";
	cin>>n;
	for( i= 0; i< n; i++ )
	{
		cout<<"\nprocess "<<(i+1)<<endl;
		cout<<"\nenter process id : ";
		cin>>Pid;
		cout<<"enter arrival time : ";
		cin>>arr_time;
		cout<<"enter burst time : ";
		cin>>bur_time;
		cout<<"enter priority of the process : ";
		cin>>Priority;
		t1.Num = i+1;
		t1.arr_time = arr_time;
		t1.bur_time = bur_time;
		t1.rem_time = bur_time;
		t1.Pid = Pid;
		t1.Priority = Priority;
		input.push_back(t1);
		cout<<"\n";
	}
	input_copy = input;
	sort( input.begin(), input.end(), arrivalsort );
    total_exection_time = total_exection_time + input[0].arr_time;
    for( i= 0 ;i< n; i++ )
    {
    	if( total_exection_time >= input[i].arr_time )
    	{
    		total_exection_time = total_exection_time +input[i].bur_time;
    	}
    	else
    	{
    		int diff = (input[i].arr_time - total_exection_time);
    		total_exection_time = total_exection_time + diff + bur_time;

    	}
    }

	int Ghant[total_exection_time]={0};                          //Ghant Chart
	for( i= 0; i< total_exection_time; i++ )
	{
		Ghant[i]=-1;
	}

	priority_queue < temp ,vector<Process_Data> ,comp> level1_q; //Priority Queue 1st level queue

	queue< temp > level2_q;                                      //Round Robin Queue 2nd level queue
	int cpu_state = 0;                                           //idle if 0 then Idle if 1 the Busy
	int quantum = 2 ;                                            //Time Quantum
	cur.Pid = -2;
	cur.Priority = 999999;

	for ( clock = 0; clock< total_exection_time; clock++ )
	{
		
		for( int j = 0; j< n ; j++ )
		{
			if(clock == input[j].arr_time)
			{
				level1_q.push(input[j]);
			}
		}
		

		if(cpu_state == 0)                      //If CPU is idle
		{
			if(!level1_q.empty())
			{
				cur = level1_q.top();
				cpu_state = 1;
				level1_q_process = 1;
				level1_q.pop();
				quantum = 2; 
			}
			else if(!level2_q.empty())
			{
				cur = level2_q.front();
				cpu_state = 1;
				level2_q_process = 1;
				level2_q.pop();
				quantum = 2;
			}
		}
		else if(cpu_state == 1)                                 //If cpu has any process i.e.,cpu not idle
		{
			if(level1_q_process == 1 && (!level1_q.empty()))
			{
				if(level1_q.top().Priority < cur.Priority )     //If new process has high priority
				{
					level2_q.push(cur);                         //push cur in 2nd level queue
					cur = level1_q.top();
					level1_q.pop();
					quantum = 2; 
				}
			}
			else if(level2_q_process == 1 && (!level1_q.empty()))    //If process is from 2nd level queue and new process come  in 1st level queue
			{
				level2_q.push(cur);
				cur = level1_q.top();
				level1_q.pop();
				level2_q_process = 0;
				level1_q_process = 1;
				quantum = 2 ;
			}
			

		}


		if(cur.Pid != -2)                         // Process Execution
		{
			cur.rem_time--;
			quantum--;
			Ghant[clock] = cur.Pid;
			if(cur.rem_time == 0)                 //If process Finish
			{
				cpu_state = 0 ;
				quantum = 2 ;
				cur.Pid = -2;
				cur.Priority = 999999;
				level2_q_process = 0;
				level1_q_process = 0;
			}
			else if(quantum == 0 )                //If time Qunatum of a current running process Finish
			{
				level2_q.push(cur);
				cur.Pid = -2;
				cur.Priority = 999999;
				level2_q_process = 0;
				level1_q_process = 0;
				cpu_state=0;

			}

		}
		
	}


	sort( input.begin(), input.end(), idsort );
	
	for(int i=0;i<n;i++)
	{
		for(int k=total_exection_time;k>=0;k--)
		{
			if(Ghant[k]==i+1)
			{
				input[i].end_time=k+1;
				break;

			}
		}
	}
	for(int i=0;i<n;i++)
	{
		for(int k=0;k<total_exection_time;k++)
		{

			if(Ghant[k]==i+1)
			{
				input[i].start_time=k;
				break;
			}
		}
	}
	
	sort( input.begin(), input.end(), Numsort );

	for(int i=0;i<n;i++)
	{
		input[i].Res_time=input[i].start_time-input[i].arr_time;
		input[i].idle_time=(input[i].end_time-input[i].arr_time)-input[i].bur_time;

	}
	
	for(int i=0;i<n;i++)
	{
		cout<<"\n\n\nprocess "<<(i+1);
		cout<<"\nprocess id is : "<<input[i].Pid<<endl;
		cout<<"response time of the process is : "<<input[i].Res_time<<endl;
		cout<<"finish time of the process is : "<<input[i].end_time<<endl;
		cout<<"idle time of the process : "<<input[i].idle_time<<endl;
		
	}	
	return 0;
}
