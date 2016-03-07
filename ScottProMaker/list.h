#include "node.h"

class lList{
	public:
	   node* head;
	   node* tail;
	   node* current;
	  
	//public:
	   void addLeft(int num);
	   void addRight(int num);
	   void left();
	   void right();
	   void search(int num);
	   void print();
	   int getCycles();
	   int getMemSize();
	   void setMean(int num);
	   void getTotal();
};