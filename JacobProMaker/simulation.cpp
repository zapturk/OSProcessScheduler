/*
 * Jacob Cole
 * simulation.cpp
 *
 * Simulates the creation of K processes and gives them an amount of cycles and memory within a specified range
 *
 */
#include <iostream>
#include <time.h>
#include <map>
#include "stdlib.h"
using namespace std;

const int MINCYCLES = 1000;
const int MAXCYCLES = 11000;
const int MINMEMORY = 1;
const int MAXMEMORY = 100;
const int CS = 10;

class process
{
public:
	int id;
	int numOfCycles;
	int memorySize;
	int waitTime;
	int arrivalTime;
	int processTimes;

	bool operator<(const process& other) const
	{
		return id < other.id;
	}
};

map <int, process> processMap;

void simulate(int );
void fifo(int );
void sjf(int );
void roundRobin(int );

int main()
{
	int i;

	cout << "Enter number of processes to simulate: ";
	cin >> i;

	simulate(i);
	fifo(i);
	sjf(i);
	roundRobin(i);
	

	return 0;
}

void simulate(int numOfProcesses)
{
	cout << "Simulating creation of " << numOfProcesses << " processes..." << endl;

	//available resources to keep within the mean
	int totalCycles = numOfProcesses * 6000;
	int totalMemory = numOfProcesses * 20;

	int calcAvgCycles = 0;
	int calcAvgMemory = 0;

	srand(time(NULL));

	for(int i=1; i <= numOfProcesses; i++)
	{
	 	process p;
		p.id = i;
		p.arrivalTime = (i-1) * 50;
		p.processTimes = 0;

		if(numOfProcesses == 1) {//directly assign the means
			p.numOfCycles = 6000;
			p.memorySize = 20;

		} else {
			//assign rest of total if on last process
			if(numOfProcesses == i && totalCycles >= MINCYCLES && totalMemory >= MINMEMORY) {
				p.numOfCycles = totalCycles;
				p.memorySize = totalMemory;
			}
			//not on last process
			else {
				if(totalCycles > MAXCYCLES) {
					p.numOfCycles = rand() % MAXCYCLES + MINCYCLES;
					totalCycles = totalCycles - p.numOfCycles;
				}
				else {
					if(totalCycles == 0) {
						cout << "Error: Ran out of cycles" << endl;
					} else {
						p.numOfCycles = rand() % (totalCycles -  MINCYCLES*(numOfProcesses - i)) + MINCYCLES;//ensure max leaves enough cycles for the rest to be at least MIN
						totalCycles = totalCycles - p.numOfCycles;
					}
				}

				if(totalMemory > MAXMEMORY) {
					p.memorySize = rand() % MAXMEMORY + MINMEMORY;
					totalMemory = totalMemory - p.memorySize;
				}
				else {
					if(totalMemory == 0) {
						cout << "Error: Ran out of memory" << endl;
					} else {
						p.memorySize = rand() % (totalMemory - MINMEMORY*(numOfProcesses - i)) + MINMEMORY;//ensure max leaves enough memory for the rest to be at least MIN
						totalMemory = totalMemory - p.memorySize;
					}
				}
		    }
		}

		processMap[p.id] = p;

		calcAvgMemory += p.memorySize;
		calcAvgCycles += p.numOfCycles;

		cout << "pid = " << p.id << ", cycles = " << p.numOfCycles << ", memory(KB) = " << p.memorySize << ", arrival time = " << p.arrivalTime << endl;
		//cout << "pid = " << processMap[i].id << ", cycles = " << processMap[i].numOfCycles << ", memory(KB) = " << processMap[i].memorySize <<"should be same as above"<< endl;
	}

	cout << "Avg Cycles: " << calcAvgCycles / numOfProcesses << " Avg Memory: " << calcAvgMemory / numOfProcesses << endl;

}


void sjf(int i)
{
	int arrayFinished[51][3];
	int overallTime = 0, processArrived = 0, fastestProcess = 2, j = 0, count = 1, timeRun = 0, waitTime = 0, averageWait = 0;

	cout << endl << "---SJF Algorithm---" << endl;

	while (count <= i)
	{
        	arrayFinished[count][0] = count;			//I used whiles to populate the 2d array
        	arrayFinished[count][1] = 0;
        	count++;
        }

	while(j < i)
	{
		fastestProcess = 2;
		if (overallTime == 0)
		{
			timeRun = processMap[1].numOfCycles;
			arrayFinished[1][1] == 1;
			fastestProcess = 1;
			//cout << "pid = " << processMap[1].id << ", waitTime = 0" << ", totalTime = " << processMap[1].numOfCycles << endl;
		}
		else if(overallTime != 0)
		{
			processArrived = overallTime/50;
			if (processArrived > i)
			{
				processArrived = i;
			}

			int k = 2;
			int tempLarge = 15000;
			while(k <= processArrived)
			{
				if(processMap[k].numOfCycles <= tempLarge/* && arrayFinished[k][1] == 0*/)
				{
					if(arrayFinished[k][1] == 0)
					{
						fastestProcess = k;
						tempLarge = processMap[k].numOfCycles;
						k++;
					}
					else
					{
						k++;
					}
				}
				else
				{
					k++;
				}

			}
			timeRun = processMap[fastestProcess].numOfCycles;
			arrayFinished[fastestProcess][1] = 1;
		}
		waitTime = (overallTime - (50*j)+(10*j)/*processMap[fastestProcess].arrivalTime*/);
		overallTime = overallTime + timeRun;
		cout << "Pid = " << processMap[fastestProcess].id  << ", Cycles = " << processMap[fastestProcess].numOfCycles  << ", Total Time = " << overallTime << ", Wait Time = " << waitTime  << endl;
		//cout << "pid = " << processMap[fastestProcess].id << ", waitTime = "  << =-=-=-=-=-=  << ", totalTime = " << overallTime << endl;
		j++;
	}
	averageWait = waitTime/i;
	cout << "Average wait time = " << waitTime << endl;
	cout << "Total penalty time = " << (i-1)*10 << endl;
	cout << "------" << endl;
}

void fifo(int numOfProcesses)
{
	process tempP;
	int totalWait = 0;
	int overallTime = 0;

	cout << endl << "---FIFO Algorithm---" << endl;

	for(int i=1; i <= numOfProcesses; i++)
	{
		tempP = processMap[i];
		processMap[i].waitTime = tempP.waitTime = overallTime;
		overallTime = overallTime + tempP.numOfCycles;
		totalWait = totalWait + processMap[i].waitTime;

		cout << "Pid = " << tempP.id << ", Cycles = " << tempP.numOfCycles <<", Wait Time = " << tempP.waitTime << endl;

		//add context switch time to overall if not on last process
		if(i != numOfProcesses) {
			overallTime = overallTime + CS;
		}
	}

	cout << "Average wait time = " << totalWait / numOfProcesses << endl;
	cout << "Total penalty time = " << (numOfProcesses-1) * CS << endl;
	cout << "------" << endl;
}

void roundRobin(int i){
	int oTime = 0;
	int x = 1, a = 0, y;
	int lastP = 1;
	int cs = 0;
	int avgWait = 0;
	int count;
	int cycl[50];

	for(count = 0; count < i; count++)
		cycl[count] = processMap[count+1].numOfCycles;

	while(a != i){
		if(processMap[x].numOfCycles > 0){
			if(processMap[x].numOfCycles > 50){
				if(lastP != x){
					oTime += 10;
					cs++;
					cout << "Context Switch number = " << cs << ", Overall Time = " << oTime << endl;
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes));
				processMap[x].numOfCycles -= 50;
				oTime += 50;
				processMap[x].processTimes++;
				cout << "Pid = " << processMap[x].id << ", Cycles Left = " << processMap[x].numOfCycles << ", Wait Time = " << processMap[x].waitTime << endl;
				if(x == 50){
					lastP = 50;
					x = 1;
				}
				else{
					lastP = x;
					x++;
				}
			}
			else{
				if(lastP != x){
					oTime += 10;
					cs++;
					cout << "Context Switch number = " << cs << ", Overall Time = " << oTime << endl;
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes));
				oTime += processMap[x].numOfCycles;
				processMap[x].numOfCycles -= processMap[x].numOfCycles;
				a++;
				cout << "Pid = " << processMap[x].id << ", Cycles Left = " << processMap[x].numOfCycles << ", Wait Time = " << processMap[x].waitTime << endl;
				if(x == 50){
					lastP = 50;
					x = 1;
				}
				else{
					lastP = x;
					x++;
				}
			}
		}
		else{
			if(x == 50)
				x = 1;
			else
				x++;
		}
	}

	cout << endl << "---Round Robin Algorithm---" << endl;

	//cout << "over all time: " << oTime << endl;
	//cout << "number of context switches: " << cs << endl;
	
	for(y = 1; y <= i; y++){
		cout << "Pid = " << processMap[y].id << ", Cycles = " << cycl[y-1] << ", Wait Time = " << processMap[y].waitTime << endl;
		avgWait += processMap[y].waitTime;
	}
	cout << "Average wait time = " << avgWait/i << endl;
	cout << "Total penalty time = " << cs * 10 << endl;
	cout << "------" << endl;
}
