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
void roundRobin(int );
void rrmp(int );
void test(int );
int main()
{
	int i;

	cout << "Enter number of processes to simulate: ";
	cin >> i;

	simulate(i);
	//roundRobin(i);
	rrmp(i);

	return 0;
}

void rrmp(int i){
	int oTime[4] = {0,50,100,150};
	int x = 1, a = 0, y;
	int lastP[4];
	int cs = 0;
	int wRun = 0;
	int avgWait = 0;
	int numRun = 0;
	int thread[4] = {0,0,0,0};
	int numCPU = 0;
	int doneP[i];
	int tat;

	for(y = 0; y < i; y++)
		doneP[y] = 0;

	while(a != i){

		if(numRun < i){
			numRun++;
			thread[numCPU] = numRun; // set the cpu to a process
			lastP[numCPU] = numRun;// set the last process on cpu to same number
			if(wRun > 4){
				oTime[numCPU] += 10;// if there is a switch in process do a contex switch
				cs++;
				//cout << "context switche: " << cs << endl;
			}
				
			if(numCPU == 3)
				numCPU = 0; // sets the cpu back CPU 0
			else
				numCPU++; // gose up one CPU
		}
		else if(i > 4){
			for(numCPU = 0; numCPU < 4; numCPU++){ // after seting up all the process switch every cycle
				y = lastP[numCPU];
				if(y + 4 > i)
					y = numCPU + 1;// check to make sure to call a process on the map
				else
					y += 4;
				tat = 0;
				while(doneP[y] == 1 && tat < 2){// check for next process that is not done
					if(y + 4 > i){
						y = numCPU + 1;
						tat++;
					}
					else
						y += 4;
				}
				if(tat > 1)
					y = 0;
				if(y != lastP[numCPU]){// if a new porcess is loaded update the over all time with a context switch 
					thread[numCPU] = y;// and the last process number for that cpu 
					lastP[numCPU] = y;
					oTime[numCPU] += 10;
					cs++;
				}
			}
		}


		for(y = 0; y < 4; y++){
			cout << "CPU" << y+1 << ": ";
			if(processMap[thread[y]].numOfCycles > 50){
				processMap[thread[y]].waitTime = oTime[y] - (processMap[thread[y]].arrivalTime + (50 * processMap[thread[y]].processTimes));
				test(thread[y]);
				oTime[y] += 50;
				cout << " | ";
				if(y == 3)
					cout << endl;
			}
			else if(thread[y] == 0){
				cout << "Empty | ";
				if(y == 3)
					cout << endl;
			}
			else{
				processMap[thread[y]].waitTime = oTime[y] - (processMap[thread[y]].arrivalTime + (50 * processMap[thread[y]].processTimes));
				oTime[y] += processMap[thread[y]].numOfCycles;
				test(thread[y]);
				doneP[thread[y]] = 1;
				a++;
				thread[y] = 0;
				cout << " | ";
				if(y == 3)
					cout << endl;
			}
		}
		wRun++;
	}
	cout << "oTime1 = " << oTime[0] << ", oTime2 = " << oTime[1] << ", oTime3 = " << oTime[2] << ", oTime4 = " << oTime[3] << endl;
}

void test(int tProcess){
	if(processMap[tProcess].numOfCycles > 50){
		processMap[tProcess].numOfCycles -= 50;
		processMap[tProcess].processTimes++;
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
	else{
		processMap[tProcess].numOfCycles -= processMap[tProcess].numOfCycles;
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
}

void roundRobin(int i){
	int oTime = 0;
	int x = 1, a = 0, y;
	int lastP = 1;
	int cs = 0;
	int avgWait = 0;
	while(a != i){
		if(processMap[x].numOfCycles > 0){
			if(processMap[x].numOfCycles > 50){
				if(lastP != x){
					oTime += 10;
					cs++;
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes));
				processMap[x].numOfCycles -= 50;
				oTime += 50;
				processMap[x].processTimes++;
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
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes));
				oTime += processMap[x].numOfCycles;
				processMap[x].numOfCycles -= processMap[x].numOfCycles;
				a++;
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
	cout << "over all time: " << oTime << endl;
	cout << "number of context switches: " << cs << endl;
	cout << "time of the context switches: " << cs * 10 << endl;
	for(y = 1; y <= i; y++){
		cout << "pid: " << processMap[y].id << " wait time: " << processMap[y].waitTime << endl;
		avgWait += processMap[y].waitTime;
	}
	cout << "avg wait time is: " << avgWait/i << endl;
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
		p.waitTime = 0;

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