#include <iostream>
using namespace std;

class node{

  friend class lList;
  
  private:
    int ID;
	int cycles;
	int memSize;
    node *next;
    node *pre;
   
  public:
    node(int pID, int cyc, int mem);
};