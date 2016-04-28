/*
 * Jacob Cole, Scott McKeefer, Mark Doggendorf
 * simulation.cpp
 *
 * Simulates the creation of K processes and gives them an amount of cycles and memory within a specified range
 *
 */
#include <iostream>
#include <time.h>
#include <map>
#include <cmath>
#include <string>
#include <stdio.h>
#include "stdlib.h"
using namespace std;

const int MINCYCLES = 1000;
const int MAXCYCLES = 11000;
const int MINMEMORY = 1;
const int MAXMEMORY = 100;
const int CS = 10;//context switch penalty

class process
{
public:
	int id;
	int numOfCycles;
	int memorySize;
	int waitTime;
	int arrivalTime;
	int processTimes;
	void *memBlock;

	bool operator<(const process& other) const
	{
		return id < other.id;
	}

	void initVars()
	{
		id = 0;
		numOfCycles = 0;
		memorySize = 0;
		waitTime = 0;
		arrivalTime = 0;
		processTimes = 0;
	}
};

map <int, process> processMap;

int processorInUse[51][5];//if using, call clearProcessors() when your function is done
int setNum = 1;

void clearProcessors();
void simulate(int );
void fifo(int );
void fifoMulti(int );
void sjf(int );
void sjfMulti(int i);
void roundRobin(int );
void roundRobinMulti(int );
void processEdit(int );
int getMemSize(int );
void sysMF(int );
void ourMallocAndFree(int );

int ourFree(int , int);


int main()
{
	int i = 64;//use 50 processes

	clearProcessors();

	//run of first set of processes
	simulate(i);
	sysMF(i);
	ourMallocAndFree(i);

	//fifo(i);
	//fifoMulti(i);
	//sjf(i);
	//sjfMulti(i);
	//roundRobin(i);
	//roundRobinMulti(i);

	//processMap.clear();//clear map for next set of processes

	//run on 2nd set of processes
	//simulate(i);
	//fifo(i);
	//fifoMulti(i);
	//sjf(i);
	//sjfMulti(i);
	//roundRobin(i);
	//roundRobinMulti(i);
	
	return 0;
}

//reset processor array to empty
void clearProcessors() 
{
	for(int i=1; i <= 64; i++) {
		for(int j=1; j <= 4; j++){
			processorInUse[i][j] = 0;
		}
	}	
}

//generate set of n processes
void simulate(int numOfProcesses)
{
	cout << "Simulating creation of " << numOfProcesses << " processes..." << endl;

	//available resources to keep within the mean
	int totalCycles = numOfProcesses * 6000;
	int totalMemory = numOfProcesses * 20;

	int calcAvgCycles = 0;
	int calcAvgMemory = 0;

	if(setNum == 1) 
		srand(time(NULL));
		setNum++;
	/*}
	else {
		srand(3);
	}*/

	for(int i=1; i <= numOfProcesses; i++)
	{
		process p;
		p.initVars();
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



int getMemSize(int numOfProcesses){
	int num, total = 0;
	for(num = 1; num <= numOfProcesses; num++){
		total += processMap[num].memorySize;
	}
	return(total);
}

void ourMallocAndFree(int numOfProcesses)
{
	clock_t start, finish;
	int num;
	int memoryBlock[20000];
	int blockSize = 0;
	int memoryBlockLimit = 20000;//20MB converted to KB
	int startAvail = -1;
	int endAvail = -1;

	//initialize all 20MB as free
	for(int i=0; i < 20000; i++){
		memoryBlock[i] = 0;
	}

	cout << endl << "----- Part 2 -----" << endl;

	start = clock();
	for(num = 1; num <= numOfProcesses; num++){

		//start our malloc
		//get block range
		for(int i=0; i<20000; i++){
			if(memoryBlock[i] == 0) {//memory available
				//cout << "free block = " << i << endl;
				if(blockSize < processMap[num].memorySize) {
					if(startAvail == -1) {
						startAvail = i;
					}
					blockSize++;
				}
				if(blockSize >= processMap[num].memorySize){
					endAvail = i;
					cout << "Process " << num << " has been malloced." << endl;
					break;
				}
			}		
		}

		//cout << "startAvail =" << startAvail << endl;
		//cout << "endAvail = " << endAvail << endl;
		//allocate block range
		for(int i=startAvail; i <= endAvail; i++){
			memoryBlock[i] = 1;
		}
		//end our malloc



		//start our free
		for(int i=startAvail; i <= endAvail; i++) {
			memoryBlock[i] = 0;
		}
		cout << "\tProcess " << num << "has been freed." << endl;
		//end our free

		//reset
		startAvail = -1;
		endAvail = -1;
		blockSize = 0;
	}

	finish = clock();
	cout << "Using custom malloc/free: " << (double)(finish-start) / CLOCKS_PER_SEC << endl;
}

void sysMF(int numOfProcesses){
	clock_t start, finish;
	int big, done = 0 , x, min;
	int num;

	cout << endl << "	----Part 1----" << endl;
	start = clock();
	for(num = 1; num <= numOfProcesses; num++){
		processMap[num].memBlock = malloc(processMap[num].memorySize);
		cout << "Process " << num << " has been malloced." << endl;
		for(x=1; x<= num; x++){
			if(processMap[num].numOfCycles > 50){
				processMap[num].numOfCycles -= 50;
			}
			else if(processMap[num].numOfCycles > 0){
				processMap[num].numOfCycles -= processMap[num].numOfCycles;
				free(processMap[num].memBlock);
				done++;
				cout << "	Process " << x << " has been freed." << endl;
			}
		}
	}

	while(done != numOfProcesses){
		big = 50000;

		//find the smallest cycle left
		for(num = 1; num <= numOfProcesses; num++){
			if(processMap[num].numOfCycles > 0){
				if(processMap[num].numOfCycles < big){
					min = num;
					big = processMap[num].numOfCycles;
				}
			}
		}

		//take min from cycle left and free mem
		for (x = 1; x <= numOfProcesses; x++){
			if(min == x){
				processMap[x].numOfCycles -= processMap[x].numOfCycles;
				free(processMap[x].memBlock);
				done++;
				cout << "	Process " << x << " has been freed." << endl;
			}
			else if(processMap[x].numOfCycles > 0){
				processMap[x].numOfCycles -= processMap[min].numOfCycles;
				if(processMap[x].numOfCycles == 0){
					free(processMap[x].memBlock);
					done++;
					cout << "	Process " << x << " has been freed." << endl;
				}
			}
		}
	}


	finish = clock();
	cout << "Using system malloc: " << (double)(finish-start) / CLOCKS_PER_SEC << endl;
}

























void sjf(int i)
{
	int arrayFinished[51][2];
	int overallTime = 0, processArrived = 0, fastestProcess = 2, j = 0, count = 1, timeRun = 0, waitTime = 0, totalWait = 0, averageWait = 0;

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
			timeRun = processMap[1].numOfCycles;   //calculate the first process runtime
			fastestProcess = 1;
		}
		else if(overallTime != 0)
		{
			processArrived = overallTime/50;		//Calculate the amount of arrived proccesses
			if (processArrived > i)
			{
				processArrived = i;
			}

			int k = 2;
			int tempLarge = 15000;
			while(k <= processArrived)
			{
				if(processMap[k].numOfCycles <= tempLarge && arrayFinished[k][1] == 0)   //find the smallest of the arrived proccesses
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
			timeRun = processMap[fastestProcess].numOfCycles;		//accumulate runtime of proccesses
			arrayFinished[fastestProcess][1] = 1;
		}
		waitTime = (overallTime - (50*j)+(10*j));
		totalWait = totalWait + waitTime;				//calculate averages
		overallTime = overallTime + timeRun;
			if(j != 0)
			{
				overallTime = overallTime+10;
			}
		cout << "Pid = " << processMap[fastestProcess].id  << ", Cycles = " << processMap[fastestProcess].numOfCycles  << ", Wait Time = " << waitTime  << endl;
		j++;
	}
	averageWait = totalWait/i;
	cout << "Average wait time = " << averageWait << endl;
	cout << "Total penalty time = " << (i-1)*10 << endl;
	cout << "------" << endl;
}

void sjfMulti(int i)
{
	int arrayFinished[51][2], P[4] = {0};
	int overallTime = 0, processArrived = 0, fastestProcess = 2, j = 4, count = 0, timeRun = 0, waitTime = 0, totalWait = 0, averageWait = 0, P1Wait = 0, P2Wait = 0, P3Wait = 0, P4Wait = 0;
	int optimalProcessor = 1, freeProcessor = 1, start = 0;

	cout << endl << "---SJF Multi Processors Algorithm---" << endl;

	while (count <= i)
	{
        	arrayFinished[count][0] = count;			//I used whiles to populate the 2d array
        	arrayFinished[count][1] = 0;
        	count++;
        }

	while(j <= i)//j=4  goes from 4 to 50. one run will be the initial 4 processors being filled. 
	{		//That means it will technically run the else loop from 5->50
		fastestProcess = 5;
		if (start == 0)
		{
			for(count = 0; count < 4; count++)
			{
				if(processMap[count+1].numOfCycles < processMap[count+2].numOfCycles)
				{
					fastestProcess = count;
				}
				P[count] = processMap[count+1].numOfCycles + (count * 50);
				cout << "| CPU"<< count+1 << ": Pid = " << count+1 << ", Cycles = " << processMap[count+1].numOfCycles <<  ", Wait Time = " << 0 << " ";
				if(count == 3)
					cout << endl;
			}
		}
		else
		{
			int tempCheck = 1500000000;
			for(count = 0; count < 4; count++)
			{
				//cout << P[count] << endl;
				if(P[count] < tempCheck)
				{
					//optimalProcessor = P[count]; ////////////////////This is the amount of cycles on the smallest processor
					optimalProcessor = count+1;   //this is which processor has the lowest cycle count at the beginning
					processArrived = (P[count] / 50);
					if(processArrived > i)
					{
						processArrived = i;
					}
					tempCheck = P[count];
				}
			}
			int k = 5;
			int tempLarge = 15000;
			while(k <= processArrived)
			{
				if(processMap[k].numOfCycles <= tempLarge && arrayFinished[k][1] == 0)
                                {
                                        fastestProcess = k;				//check if process is smallest and has not been used
                                        tempLarge = processMap[k].numOfCycles;
                                        k++;
                                }
                                else
                                {
                                        k++;
                                }
			}
			waitTime = (P[optimalProcessor-1] - ((fastestProcess-1)*50));
			P[optimalProcessor-1] = P[optimalProcessor-1] + processMap[fastestProcess].numOfCycles + 10;
			arrayFinished[fastestProcess][1] = 1;
			cout << "| CPU" << optimalProcessor << ": Pid = " << fastestProcess << ", Cycles = " << processMap[fastestProcess].numOfCycles  <<", Wait Time = " << waitTime << endl;
			//cout << "Processor 1: " << P[0] << "	Processor 2: " << P[1] << "	Processor 3: " << P[2] << "	Processor 4: " << P[3] << endl << endl;
		}
		start = 1;
		totalWait = totalWait + waitTime;
		j++;
	}
	averageWait = totalWait/i;
	cout << "Average wait time = " << averageWait << endl;
	cout << "Total penalty time = " << (i-4)*10 << endl;
	cout << "TOTAL CYCLES on Processor 1 = " << P[0] << ", Processor 2 = " << P[1] << ", Processor 3 = " << P[2] << ", Processor 4 = " << P[3] << endl;
	cout << "------" << endl;
}

void fifo(int numOfProcesses)
{
	process tempP;
	int totalWait = 0;
	int overallTime = 0;

	cout << endl << "---FIFO Algorithm---" << endl;

	//literally go in order from 1 to 50
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

	cout << "Average wait time per process = " << totalWait / numOfProcesses << endl;
	cout << "Total penalty time = " << (numOfProcesses-1) * CS << endl;
	cout << "------" << endl;
}

//each processor is basically goig to run FIFO itself
void fifoMulti(int numOfProcesses)
{
	int totalWait1 = 0, totalWait2 = 0, totalWait3 = 0, totalWait4 = 0;
	int overallTime1 = 0, overallTime2 = 0, overallTime3 = 0, overallTime4 = 0;
	int pid = 0;
	int numRows = (ceil((float)numOfProcesses / 4.0f));//for printing purposes
	int numOfProcessesRanInCPU[5] = {0,0,0,0,0};
	map <int, process> copyMap = processMap;//make a copy so we can change values without affecting other algorithms

	cout << endl << "---FIFO Algorithm Multiprocessor---" << endl;

	//loops through each interation of what is in the processor.
	for(int i=1; pid < numOfProcesses; i++)
	{
		//processor1
		processorInUse[i][1] = copyMap[++pid].id;
		numOfProcessesRanInCPU[1]++;
		copyMap[pid].waitTime = overallTime1 - copyMap[pid].arrivalTime;
		overallTime1 = overallTime1 + copyMap[pid].numOfCycles;
		totalWait1 = totalWait1 + copyMap[pid].waitTime;
		cout << "| CPU1: "  << "pid= " << processorInUse[i][1] << ", Cycles= " << copyMap[processorInUse[i][1]].numOfCycles << ", WaitTime=" << copyMap[processorInUse[i][1]].waitTime;
		
		//in case CPU1 is the last one to print
		if(pid == numOfProcesses) {
			cout << " | " << endl;
		}

		//only add to other processors if not on the last process
		if(pid != numOfProcesses) {

			//here because we don't want to add a CS if CPU1 is done
			//same reasoning for follwing CS's
			overallTime1 = overallTime1 + CS;

			//don't calculate if on first iteration.
			//same reasoning for other processors
			if(i != 1) {
				//processor2
				processorInUse[i][2] = copyMap[++pid].id;
				numOfProcessesRanInCPU[2]++;			
				copyMap[pid].waitTime = overallTime2 - copyMap[pid].arrivalTime;
				if(copyMap[pid].waitTime < 0)//prevent time from being negative. being negative means it hasn't arrived
					copyMap[pid].waitTime = 0;
				overallTime2 = overallTime2 + copyMap[pid].numOfCycles;
				totalWait2 = totalWait2 + copyMap[pid].waitTime;
			}
			cout << " | CPU2: "  << "pid= " << processorInUse[i][2] << ", Cycles= " << copyMap[processorInUse[i][2]].numOfCycles << ", WaitTime=" << copyMap[processorInUse[i][2]].waitTime;

			if(processorInUse[i][2] != 0)
				overallTime2 = overallTime2 + CS;

			if(i != 1) {
				//processor3
				processorInUse[i][3] = copyMap[++pid].id;
				numOfProcessesRanInCPU[3]++;
				copyMap[pid].waitTime = overallTime3 - copyMap[pid].arrivalTime;
				if(copyMap[pid].waitTime < 0)//prevent time from being negative
					copyMap[pid].waitTime = 0;			
				overallTime3 = overallTime3 + copyMap[pid].numOfCycles;
				totalWait3 = totalWait3 + copyMap[pid].waitTime;
			}
			cout << " | CPU3: "  << "pid= " << processorInUse[i][3] << ", Cycles= " << copyMap[processorInUse[i][3]].numOfCycles << ", WaitTime=" << copyMap[processorInUse[i][3]].waitTime;

			if(processorInUse[i][3] != 0)
				overallTime3 = overallTime3 + CS;

			if(i != 1) {
				//processor4
				processorInUse[i][4] = copyMap[++pid].id;
				numOfProcessesRanInCPU[4]++;
				copyMap[pid].waitTime = overallTime4 - copyMap[pid].arrivalTime;
				if(copyMap[pid].waitTime < 0)//prevent time from being negative.
					copyMap[pid].waitTime = 0;			
				overallTime4 = overallTime4 + copyMap[pid].numOfCycles;
				totalWait4 = totalWait4 + copyMap[pid].waitTime;
			}
			cout << " | CPU4: "  << "pid= " << processorInUse[i][4] << ", Cycles= " << copyMap[processorInUse[i][4]].numOfCycles << ", WaitTime=" << copyMap[processorInUse[i][4]].waitTime << " | " << endl;

			if(processorInUse[i][4] != 0)
				overallTime4 = overallTime4 + CS;
		}

	}

	int averageWaitCPU1 = totalWait1 / numOfProcessesRanInCPU[1];
	int averageWaitCPU2 = totalWait2 / numOfProcessesRanInCPU[2];
	int averageWaitCPU3 = totalWait3 / numOfProcessesRanInCPU[3];
	int averageWaitCPU4 = totalWait4 / numOfProcessesRanInCPU[4];

	/* not using to be uniform across all algorithms. kind of not needed anyways.
	cout << "Average wait time per process for CPU1 = " << averageWaitCPU1 << endl;
	cout << "Average wait time per process for CPU2 = " << averageWaitCPU2 << endl;
	cout << "Average wait time per process for CPU3 = " << averageWaitCPU3 << endl;
	cout << "Average wait time per process for CPU4 = " << averageWaitCPU4 << endl;
	*/	

	cout << "Total penalty time = " << (numOfProcesses-4) * CS << endl;
	cout << "Average wait time per process of the entire system = " 
		<< ((averageWaitCPU1 + averageWaitCPU2 + averageWaitCPU3 + averageWaitCPU4)/4) << endl;
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

	cout << endl << "---Round Robin Algorithm---" << endl;

	for(count = 0; count < i; count++)
		cycl[count] = processMap[count+1].numOfCycles; // keeping a record of all the cycle numbers

	while(a != i){
		if(processMap[x].numOfCycles > 0){
			if(processMap[x].numOfCycles > 50){ // if the pocess is not done and it is has more than 50 left
				if(lastP != x){// if its a new process do a context switch first
					oTime += 10;// add time to over all
					cs++;
					cout << "Context Switch number = " << cs << ", Overall Time = " << oTime << endl;
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes));// get wait time
				processMap[x].numOfCycles -= 50;// take off cycles
				oTime += 50; // add time to over all time
				processMap[x].processTimes++; // updating the number of time the process has been worked on
				cout << "Pid = " << processMap[x].id << ", Cycles Left = " << processMap[x].numOfCycles << ", Wait Time = " << processMap[x].waitTime << endl;
				if(x == 50){
					lastP = 50; // go back to first process if at last
					x = 1;
				}
				else{
					lastP = x;// go up by one process
					x++;
				}
			}
			else{
				if(lastP != x){
					oTime += 10;
					cs++;
					cout << "Context Switch number = " << cs << ", Overall Time = " << oTime << endl;
				}
				processMap[x].waitTime = oTime - (processMap[x].arrivalTime + (50 * processMap[x].processTimes)); // set the wait time
				oTime += processMap[x].numOfCycles;// updat the over all time
				processMap[x].numOfCycles -= processMap[x].numOfCycles;// set the cycle to 0
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

	cout << endl;
	
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

	cout << endl << "---Round Robin Algorithm Multiprocessor---" << endl;

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
						tat++;// cheack for looping more that one time
					}
					else
						y += 4;
				}
				if(tat > 1)
					y = 0;
				if(y != lastP[numCPU]){// if a new porcess is loaded update the over all time with a context switch 
					thread[numCPU] = y;// and the last process number for that cpu 
					lastP[numCPU] = y;
					oTime[numCPU] += 10;// adding the context switch
					cs++;
				}
			}
		}


		for(y = 0; y < 4; y++){
			cout << "| CPU" << y+1 << ": ";
			if(processMap[thread[y]].numOfCycles > 50){
				processMap[thread[y]].waitTime = oTime[y] - (processMap[thread[y]].arrivalTime + (50 * processMap[thread[y]].processTimes));
				processEdit(thread[y]);// edit the process cycle time 
				processMap[thread[y]].processTimes++; // updating the number of time the process has been worked on
				oTime[y] += 50; // updating the processor over all time
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
				processEdit(thread[y]);// edit the process cycle time 
				doneP[thread[y]] = 1; // set the porcess a done
				a++;
				thread[y] = 0;
				cout << " | ";
				if(y == 3)
					cout << endl;
			}
		}
	}

	cout << endl;
	for(y = 1; y <= i; y++){
		cout << "Pid = " << processMap[y].id << ", Cycles = " << cycl[y-1] << ", Wait Time = " << processMap[y].waitTime << endl;
		avgWait += processMap[y].waitTime;// calc the ave wait time
	}

	cout << "Total penalty time = " << cs * 10 << endl;
	cout << "Average wait time = " << avgWait/i << endl;
	cout << "TOTAL CYCLES on Processor 1 = " << oTime[0] << ", Processor 2 = " << oTime[1] << ", Processor 3 = " << oTime[2] << ", Processor 4 = " << oTime[3] << endl;
	cout << "------" << endl;
	for(wRun = 0; wRun < i; wRun++)
		processMap[wRun+1].numOfCycles = cycl[wRun]; // reset all of the cycle time
}

void processEdit(int tProcess){
	if(processMap[tProcess].numOfCycles > 50){
		processMap[tProcess].numOfCycles -= 50; // take 50 away from the cycle tiem
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
	else{
		processMap[tProcess].numOfCycles -= processMap[tProcess].numOfCycles; // set the cycle time to zero
		cout << "Pid = " << processMap[tProcess].id << ", Cycles Left = " << processMap[tProcess].numOfCycles << ", Wait Time = " << processMap[tProcess].waitTime;
	}
}