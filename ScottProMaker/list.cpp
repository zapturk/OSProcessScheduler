#include <iostream>
#include "list.h"
#include <time.h>
#include <stdlib.h>
using namespace std;
 
 int size = 0;
 int lsize = 0;
 int rsize = 0;
 int psize = 0;
 int memMean = 0;
 int cycMean = 0;

 node *templ;
 node *tempr;
 node *searchr;
 node *printr;

 
void lList::addLeft(int num)
{
    if(size == 0)
        srand(time(NULL));
    int tcyc = getCycles();
    int tmem = getMemSize();

    node *n;
 	n = new node(num, tcyc, tmem);
 	n->ID = num;
    n->cycles = tcyc;
    n->memSize = tmem;
 	if(size == 0)
 	{
 		head = n;
 		tail = n;
 		current = n;
 		n->next = NULL;
 		n->pre = NULL;
 		size++;
 	}
 	else 
    {
        templ = current->pre;
        if(templ == NULL)
        {
            head = n;
            n->next = current;
            n->pre = NULL;
            current->pre = n;
            size++;
        }
        else
        {
            n->next = current;
            n->pre = templ;
            current->pre = n;
            templ->next = n;
            size++;
        }     
        
    }
}

void lList::addRight(int num)
{
    if(size == 0)
        srand(time(NULL));
	int tcyc = getCycles();
    int tmem = getMemSize();

    node *n;
    n = new node(num, tcyc, tmem);
	n->ID = num;
    n->cycles = tcyc;
    n->memSize = tmem;
	if(size == 0)
 	{
 		head = n;
 		tail = n;
 		current = n;
 		n->next = NULL;
 		n->pre = NULL;
 		size++;
 	}
 	else
    {
        tempr = current->next;
        if(tempr == NULL)
        {
            tail = n;
            n->pre = current;
            n->next = NULL;
            current->next = n;
            size++;
        }
        else
        {
            n->pre = current;
            n->next = tempr;
            current->next = n;
            tempr->pre = n;
            size++;
        }
    }
}

void lList::left()
{
    
    if(size == 0)
    {
        cout << "Error! " << endl;
    }
    else if(current->pre != NULL)
    {
        current = current->pre;
        cout << current->ID << endl;
    }
    else
    {
    	cout << "Error! End of list reached." << endl;
    }

}

void lList::right()
{
    
    if(size == 0)
    {
        cout << "Error! " << endl;
    }  
    else if(current->next != NULL)
    {
        current = current->next;
        //cout << current->ID << endl;
    }
    else
    {
     	cout << "Error! End of list reached." << endl;
    }
}

void lList::search(int num)
{
    searchr = head;
    while(searchr != NULL)
    {
    	if(searchr->ID == num)
        {
            cout << "true" << endl;
            searchr = NULL;
        }
    	else if(searchr->next == NULL)
        {
            cout << "false" << endl;
            searchr = searchr->next;
        }
    	else
    		searchr = searchr->next;
    }
}

void lList::print()
{
	printr = head;
    while(printr != NULL)
    {
        if(size == 1)
        {
            cout << "Process number " << printr->ID << " has a cycle of " << printr->cycles << " and a memory size of " << printr->memSize << endl;
            printr = printr->next;
        }
        else if(printr->next != NULL)
        {
        	cout << "Process number " << printr->ID << " has a cycle of " << printr->cycles << " and a memory size of " << printr->memSize << endl;
        	printr = printr->next;
        }
        else
        {
            cout << "Process number " << printr->ID << " has a cycle of " << printr->cycles << " and a memory size of " << printr->memSize << endl;
            printr = printr->next;
        }
        	
    }
}

int lList::getCycles(){
    int min = 1000;
    int max = 11000;
    int mean = 6000;
    int cyc; 

    if(psize == 1)
        return(6000);
    else{
        if(size == psize - 1)
            if(cycMean > max || cycMean < min)
                return((rand() % max) + min);
            else
                return(cycMean);
        else if(cycMean < max && cycMean > 1000){
            cyc = (rand() % (cycMean - min)) + min;
            cycMean = cycMean - cyc;
            return(cyc);
        }
        else{
            cyc = (rand() % (max - min)) + min;
            cycMean = cycMean - cyc;
            return(cyc);
        }
    }
}

int lList::getMemSize(){
    int min = 1;
    int max = 100;
    int mean = 20;
    int mem;

    if(psize == 1)
        return(20);
    else{
        if(size == psize - 1)
            if(memMean > max || memMean < min)
                return(rand() % max);
            else
                return(memMean);
        else if(memMean < max && memMean > 1){
            mem = (rand() % (memMean - min)) + min;
            memMean = memMean - mem;
            return(mem);
        }
        else{
            if(rand() % 3 != 0){
                mem = ((rand() % (mean - min)) + min);
                memMean = memMean - mem;
                return(mem);
            }
            else{
                mem = ((rand() % (max - mean)) + mean);
                memMean = memMean - mem;
                return(mem);
            }
        }
    }
}

void lList::setMean(int num){
    psize = num;

    cycMean = psize * 6000;
    memMean = psize * 20;
}

void lList::getTotal(){
    int memTotal = 0;
    int cycTotal = 0;
    int count;
    printr = head;

    for(count = 0; count < psize; count++){
        memTotal = memTotal + printr->memSize;
        cycTotal = cycTotal + printr->cycles;
        printr = printr->next;
    }

    cout << "cycTotal: " << cycTotal << " memTotal: " << memTotal << endl;

}