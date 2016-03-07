#include <iostream>
#include <string> 
#include "list.h"
using namespace std;

int main()
{

  int z = 1, data;
  int numPro, x;
  string word;
  lList LL;    

  LL.head = NULL;
  LL.tail = NULL;
  LL.current = NULL;

  cout << "Enter the number of process you want to make: ";
  cin >> numPro;
  LL.setMean(numPro);

  for(x = 1; x <= numPro; x++){
    LL.addRight(x);
    if(x > 1)
      LL.right();
  }

  LL.print();
  LL.getTotal();    
      

return 0;
}