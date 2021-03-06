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
	bool allocatedFlag;
	int startAvail;
	int endAvail;

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
		allocatedFlag = false;
		startAvail = -1;
		endAvail = -2;
	}
};

map <int, process> processMap;

int processorInUse[51][5];//if using, call clearProcessors() when your function is done
int setNum = 1;
int mem2 = 0, mem3 = 0;

void clearProcessors();
void simulate(int );
void processEdit(int );
void getMemSize(int );
void sysMF(int , int );
void ourMallocAndFree(int , int);

int main()
{
	int i = 64;//use 50 processes

	clearProcessors();

	//run of first set of processes
	simulate(i);
	getMemSize(i);
	sysMF(i, 20000);//part 1
	ourMallocAndFree(i, 20000);//part 2
	/*** part 3 ***/
	ourMallocAndFree(i, mem2);
	ourMallocAndFree(i, mem3);
	sysMF(i, mem2);
	sysMF(i, mem3);
	/*** end part 3 ***/
	
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



void getMemSize(int numOfProcesses){
	int num, total = 0;
	for(num = 1; num <= numOfProcesses; num++){
		total += processMap[num].memorySize;
	}
	mem2 = total*.5;
	mem3 = total*.1;
	//return(total);
}

void ourMallocAndFree(int numOfProcesses, int numMeg)
{
	clock_t start, finish;
	int big, done = 0 , x, min;
	int num;
	char memoryBlock[numMeg];
	int blockSize = 0;
	int memoryBlockLimit = 20000;//20MB converted to KB
	//int startAvail = -1;
	//int endAvail = -1;
	map <int, process> processMapCopy;
	processMapCopy = processMap;
	int cat;
	//initialize all 20MB as free
	for(int i=0; i < numMeg; i++){
		memoryBlock[i] = 0;
	}
	if(numMeg == 20000)
		cout << endl << "----- Part 2 (Our Malloc/Free) -----" << endl;
	else if(numMeg == mem2)
		cout << endl << "----- Part 3 half memory available (Our Malloc/Free) -----" << mem2 << endl;
	else if(numMeg == mem3)
		cout << endl << "----- Part 3 10% memory available (Our Malloc/Free) -----" << mem3 << endl;
	start = clock();

		/*********************** simulation ******************/
	for(num = 1; num <= numOfProcesses; num++){
		//start our malloc
		//get block range
		for(int i=0; i<numMeg; i++){
			if(memoryBlock[i] == 0) {//memory available
				//cout << "free block = " << i << endl;
				//cout << "memoryBlock should be 0. actually is " << (int) memoryBlock[i] << endl;				
				if(blockSize < processMapCopy[num].memorySize) {
					if(processMapCopy[num].startAvail == -1) {
						processMapCopy[num].startAvail = i;
					}
					blockSize++;
				}
				if(blockSize >= processMapCopy[num].memorySize){
					processMapCopy[num].endAvail = i;
					cout << "Process " << num << " has been malloced." << endl;
					processMapCopy[num].allocatedFlag = true;
					break;
				}
			}
			if(memoryBlock[i] == 1){
				//cout << "memoryBlock should be 1. actually is " << (int)memoryBlock[i] << endl;
				blockSize = 0;
				processMapCopy[num].startAvail = -1;
			}
		}

		if(processMapCopy[num].allocatedFlag == true){
			//allocate block range
			for(int i=processMapCopy[num].startAvail; i <= processMapCopy[num].endAvail; i++){
				memoryBlock[i] = 1;
			}
			//end our malloc

			//cout << "Process " << num << " has been malloced." << endl;
		
			for(x=1; x<= num; x++){
				if(processMapCopy[x].numOfCycles > 50){
					processMapCopy[x].numOfCycles -= 50;
				}
				else if(processMapCopy[x].numOfCycles > 0){
					processMapCopy[x].numOfCycles -= processMapCopy[x].numOfCycles;
					//our free
					for(int i=processMapCopy[x].startAvail; i <= processMapCopy[x].endAvail; i++) {
						memoryBlock[i] = 0;
					}
					processMapCopy[x].startAvail = -1;
					processMapCopy[x].endAvail = -2;
					blockSize = 0;
					//end our free
					done++;
					cout << "	Process " << x << " has been freed." << endl;
				}
			}
		}
		else{
			//cout << num << " failed to malloc!!!!" << endl;
			num--;
			big = 50000;
			for(x = 1; x<= num; x++){
				if(processMapCopy[x].numOfCycles > 0){
					if(processMapCopy[x].numOfCycles < big){
						min = x;
						big = processMapCopy[x].numOfCycles;
					}
				}
			}

			for (x = 1; x <= num; x++){
				if(min == x){
					processMapCopy[x].numOfCycles -= processMapCopy[x].numOfCycles;
					//our free
					for(int i=processMapCopy[x].startAvail; i <= processMapCopy[x].endAvail; i++) {
						memoryBlock[i] = 0;
						//cout << "block " << i << " is free now" << endl;
					}
					processMapCopy[x].startAvail = -1;
					processMapCopy[x].endAvail = -2;
					blockSize = 0;
					//end our free
					done++;
					cout << "	Process " << x << " has been freed." << endl;
					//cin >> cat;

				}
				else if(processMapCopy[x].numOfCycles > 0){
					processMapCopy[x].numOfCycles -= processMapCopy[min].numOfCycles;
					if(processMapCopy[x].numOfCycles == 0){
						//our free
						for(int i=processMapCopy[x].startAvail; i <= processMapCopy[x].endAvail; i++) {
							memoryBlock[i] = 0;
						}
						processMapCopy[x].startAvail = -1;
						processMapCopy[x].endAvail = -2;
						blockSize = 0;
						//end our free						
						done++;
						cout << "	Process " << x << " has been freed." << endl;
					}
				}
			}		
		}
	}
		

	while(done != numOfProcesses){
		big = 50000;

		//find the smallest cycle left
		for(num = 1; num <= numOfProcesses; num++){
			if(processMapCopy[num].numOfCycles > 0){
				if(processMapCopy[num].numOfCycles < big){
					min = num;
					big = processMapCopy[num].numOfCycles;
				}
			}
		}

		//take min from cycle left and free mem
		for (x = 1; x <= numOfProcesses; x++){
			if(min == x){
				processMapCopy[x].numOfCycles -= processMapCopy[x].numOfCycles;
				//our free
				for(int i=processMapCopy[x].startAvail; i <= processMapCopy[x].endAvail; i++) {
					memoryBlock[i] = 0;
				}
				processMapCopy[x].startAvail = -1;
				processMapCopy[x].endAvail = -1;
				blockSize = 0;
				//end our free
				done++;
				cout << "	Process " << x << " has been freed." << endl;
			}
			else if(processMapCopy[x].numOfCycles > 0){
				processMapCopy[x].numOfCycles -= processMapCopy[min].numOfCycles;
				if(processMapCopy[x].numOfCycles == 0){
				//our free
				for(int i=processMapCopy[x].startAvail; i <= processMapCopy[x].endAvail; i++) {
					memoryBlock[i] = 0;
				}
				processMapCopy[x].startAvail = -1;
				processMapCopy[x].endAvail = -1;
				blockSize = 0;
				//end our free						
				done++;
					cout << "	Process " << x << " has been freed." << endl;
				}
			}
		}
	}
	/******** end simulation ******/

	finish = clock();
	//cout << "Using custom malloc/free: " << (double)(finish-start) / CLOCKS_PER_SEC << endl;
	printf("Using custom malloc/free: %0.6g\n", ((double)(finish-start) / CLOCKS_PER_SEC));
}

void sysMF(int numOfProcesses, int numMeg){
	clock_t start, finish;
	int big, done = 0 , x, min;
	int num;
	map <int, process> processMapCopy;
	processMapCopy = processMap;

	if(numMeg == 20000)
		cout << endl << "----- Part 1 (System Malloc/Free) -----" << endl;
	else if(numMeg == mem2)
		cout << endl << "----- Part 3 half memory available (System Malloc/Free) -----" << mem2 << endl;
	else if(numMeg == mem3)
		cout << endl << "----- Part 3 10% memory available (System Malloc/Free) -----" << mem3 << endl;
	
	start = clock();

	for(num = 1; num <= numOfProcesses; num++){
		processMapCopy[num].memBlock = malloc(processMapCopy[num].memorySize);
		cout << "Process " << num << " has been malloced." << endl;
		for(x=1; x <= num; x++){
			if(processMapCopy[x].numOfCycles > 50){
				processMapCopy[x].numOfCycles -= 50;
			}
			else if(processMapCopy[x].numOfCycles > 0){
				processMapCopy[x].numOfCycles -= processMapCopy[x].numOfCycles;
				free(processMapCopy[x].memBlock);
				done++;
				cout << "	Process " << x << " has been freed." << endl;
			}
		}
	}

	while(done != numOfProcesses){
		big = 50000;

		//find the smallest cycle left
		for(num = 1; num <= numOfProcesses; num++){
			if(processMapCopy[num].numOfCycles > 0){
				if(processMapCopy[num].numOfCycles < big){
					min = num;
					big = processMapCopy[num].numOfCycles;
				}
			}
		}

		//take min from cycle left and free mem
		for (x = 1; x <= numOfProcesses; x++){
			if(min == x){
				processMapCopy[x].numOfCycles -= processMapCopy[x].numOfCycles;
				free(processMapCopy[x].memBlock);
				done++;
				cout << "	Process " << x << " has been freed." << endl;
			}
			else if(processMapCopy[x].numOfCycles > 0){
				processMapCopy[x].numOfCycles -= processMapCopy[min].numOfCycles;
				if(processMapCopy[x].numOfCycles == 0){
					free(processMapCopy[x].memBlock);
					done++;
					cout << "	Process " << x << " has been freed." << endl;
				}
			}
		}
	}


	finish = clock();
	cout << "Using system malloc/free: " << (double)(finish-start) / CLOCKS_PER_SEC << endl;
}
