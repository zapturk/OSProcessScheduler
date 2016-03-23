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
#include <cmath>
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
int processorInUse[51][5];//if using, call clearProcessors() when your function is done



void clearProcessors();
//int checkProcessorsInUse();
void simulate(int );
void fifo(int );
void fifoMulti(int );
void sjf(int );
void roundRobin(int );
void roundRobinMulti(int );
void processEdit(int );

int main()
{
	int i;

	cout << "Enter number of processes to simulate: ";
	cin >> i;

	clearProcessors();

	simulate(i);
	fifo(i);
	fifoMulti(i);
	sjf(i);
	roundRobin(i);
	roundRobinMulti(i);
	

	return 0;
}

void clearProcessors() 
{
	for(int i=1; i <= 50; i++) {
		for(int j=1; j <= 4; j++){
			processorInUse[i][j] = 0;
		}
	}	
}
/*
int checkProcessorsInUse(int iteration)
{
	for(int j=1; j <= 4; j++)
	{
		if(processorInUse[iteration][j] == 0) {
			return j;//return processor number available
		}
	}

	return 0;
}
*/

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
		processMap[i].waitTime = tempP.waitTime = overallTime - tempP.arrivalTime;
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

void fifoMulti(int numOfProcesses)
{
	process tempP;
	int totalWait1, totalWait2, totalWait3, totalWait4 = 0;
	int overallTime1, overallTime2, overallTime3, overallTime4 = 0;
	int pid = 0;
	int numRows = (ceil((float)numOfProcesses / 4.0f));
	int numOfProcessesRanInCPU[5] = {0,0,0,0,0};
	map <int, process> copyMap = processMap;//make a copy so we can change values without affecting other algorithms

	cout << endl << "---FIFO Algorithm Multiprocessor---" << endl;

	for(int i=1; pid < numOfProcesses; i++)
	{
		//processor1
		processorInUse[i][1] = copyMap[++pid].id;
		numOfProcessesRanInCPU[1]++;
		copyMap[pid].waitTime = overallTime1 - copyMap[pid].arrivalTime;
		overallTime1 = overallTime1 + copyMap[pid].numOfCycles;
		totalWait1 = totalWait1 + copyMap[pid].waitTime;
		cout << "| CPU1: "  << "pid= " << processorInUse[i][1];
		if(pid == numOfProcesses) {
			cout << "| " << endl;
		}

		//only add to other processors if not on the last process
		if(pid != numOfProcesses) {

			overallTime1 = overallTime1 + CS;

			if(i != 1) {
			//processor2
			processorInUse[i][2] = copyMap[++pid].id;
			numOfProcessesRanInCPU[2]++;			
			copyMap[pid].waitTime = overallTime2 - copyMap[pid].arrivalTime;
			overallTime2 = overallTime2 + copyMap[pid].numOfCycles;
			totalWait2 = totalWait2 + copyMap[pid].waitTime;
			}
			cout << "| CPU2: "  << "pid= " << processorInUse[i][2];



			if(processorInUse[i][2] != 0)
			overallTime2 = overallTime2 + CS;

			if(i != 1) {
			//processor3
			processorInUse[i][3] = copyMap[++pid].id;
			numOfProcessesRanInCPU[3]++;
			copyMap[pid].waitTime = overallTime3 - copyMap[pid].arrivalTime;
			overallTime3 = overallTime3 + copyMap[pid].numOfCycles;
			totalWait3 = totalWait3 + copyMap[pid].waitTime;
			}
			cout << "| CPU3: "  << "pid= " << processorInUse[i][3];



			if(processorInUse[i][3] != 0)
			overallTime3 = overallTime3 + CS;

			if(i != 1) {
			//processor4
			processorInUse[i][4] = copyMap[++pid].id;
			numOfProcessesRanInCPU[4]++;
			copyMap[pid].waitTime = overallTime4 - copyMap[pid].arrivalTime;
			overallTime4 = overallTime4 + copyMap[pid].numOfCycles;
			totalWait4 = totalWait4 + copyMap[pid].waitTime;
			}
			cout << "| CPU4: "  << "pid= " << processorInUse[i][4] << "| " << endl;

			if(processorInUse[i][4] != 0)
			overallTime4 = overallTime4 + CS;
		}

	}
/*
	//print process information for each CPU
	int k = 0;
	int j;
	for(int i=1; k <= numOfProcesses+1; i++) {
		for(j=1; j <= 4; j++) {
			if(processorInUse[i][j] != 0) {//only increment if a process was serviced
				cout << "| CPU" << j << ": " << "pid= " << processorInUse[i][j];
				k++;
				if(j % 4 == 0)
					cout << " | " << endl;
			}
		}
	}
*/
	cout << "Average wait time for CPU1 = " << totalWait1 / numOfProcessesRanInCPU[1] << endl;
	cout << "totalWait1 = " << totalWait1 << endl;
	cout << "Average wait time for CPU2 = " << totalWait2 / numOfProcessesRanInCPU[2] << endl;
		cout << "totalWait2 = " << totalWait2 << endl;

	cout << "Average wait time for CPU3 = " << totalWait3 / numOfProcessesRanInCPU[3] << endl;
		cout << "totalWait3 = " << totalWait3<< endl;

	cout << "Average wait time for CPU4 = " << totalWait4 / numOfProcessesRanInCPU[4] << endl;
		cout << "totalWait4 = " << totalWait4 << endl;


	cout << "Total penalty time = " << (numOfProcesses-4) * CS << endl;
	cout << "------" << endl;

	clearProcessors();
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
	
	for(y = 1; y <= i; y++){
		cout << "Pid = " << processMap[y].id << ", Cycles = " << cycl[y-1] << ", Wait Time = " << processMap[y].waitTime << endl;
		avgWait += processMap[y].waitTime;
	}
	cout << "Average wait time = " << avgWait/i << endl;
	cout << "Total penalty time = " << cs * 10 << endl;
	cout << "------" << endl;
	for(count = 0; count < i; count++){
		processMap[count+1].numOfCycles = cycl[count];
		processMap[count+1].processTimes = 0;
	}
}

void roundRobinMulti(int i){
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
	int cycl[i];

	for(wRun = 0; wRun < i; wRun++)
		cycl[wRun] = processMap[wRun+1].numOfCycles; // save the cycles for each process


	for(y = 0; y < i; y++)
		doneP[y] = 0; // set the done state for every process to 0 or not done

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
			//cout << "current oTime = " << oTime[y] << ", current arrivalTime = " << processMap[thread[y]].arrivalTime << ", process times = " << processMap[thread[y]].processTimes << "      ";

			cout << "CPU" << y+1 << ": ";
			if(processMap[thread[y]].numOfCycles > 50){
				//cout << "current waitTime = " << processMap[thread[y]].waitTime << " ";
				processMap[thread[y]].waitTime = oTime[y] - (processMap[thread[y]].arrivalTime + (50 * processMap[thread[y]].processTimes));
				processEdit(thread[y]);
				processMap[thread[y]].processTimes++;
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
				processEdit(thread[y]);
				doneP[thread[y]] = 1;
				a++;
				thread[y] = 0;
				cout << " | ";
				if(y == 3)
					cout << endl;
			}
		}
	}
	for(y = 1; y <= i; y++){
		cout << "Pid = " << processMap[y].id << ", Cycles = " << cycl[y-1] << ", Wait Time = " << processMap[y].waitTime << endl;
		avgWait += processMap[y].waitTime;
	}
	cout << endl << "---Round Robin Algorithm Multiprocessor---" << endl;
	cout << "Average wait time = " << avgWait/i << endl;
	cout << "Total penalty time = " << cs * 10 << endl;
	cout << "oTime1 = " << oTime[0] << ", oTime2 = " << oTime[1] << ", oTime3 = " << oTime[2] << ", oTime4 = " << oTime[3] << endl;
	cout << "------" << endl;
	for(wRun = 0; wRun < i; wRun++)
		processMap[wRun+1].numOfCycles = cycl[wRun];
}

void processEdit(int tProcess){
	if(processMap[tProcess].numOfCycles > 50){
		processMap[tProcess].numOfCycles -= 50;
		//processMap[tProcess].processTimes++;
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
	else{
		processMap[tProcess].numOfCycles -= processMap[tProcess].numOfCycles;
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
}