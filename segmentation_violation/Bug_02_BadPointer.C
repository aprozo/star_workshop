#include <iostream>

void Bug_02_BadPointer() {

  int *Number = new int;
  *Number = 5;

  cout << *Number << endl;
  cout << *(Number + 1) << endl;

  // Defect
}