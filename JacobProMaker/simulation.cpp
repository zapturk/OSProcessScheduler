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

	bool operator<(const process& other) const
	{
		return id < other.id;
	}
};

map <int, process> processMap;

void simulate(int );

int main()
{
	int i;

	cout << "Enter number of processes to simulate: ";
	cin >> i;

	simulate(i);

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

		cout << "pid = " << p.id << ", cycles = " << p.numOfCycles << ", memory(KB) = " << p.memorySize << endl;
		//cout << "pid = " << processMap[i].id << ", cycles = " << processMap[i].numOfCycles << ", memory(KB) = " << processMap[i].memorySize <<"should be same as above"<< endl;
	}

	cout << "Avg Cycles: " << calcAvgCycles / numOfProcesses << " Avg Memory: " << calcAvgMemory / numOfProcesses << endl;

}