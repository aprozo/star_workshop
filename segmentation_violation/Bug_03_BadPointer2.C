#include <iostream>
#include <string>

void Bug_03_BadPointer2() {

  string *Word = new string;
  *Word = "Tohle je původní věta";

  cout << *Word << endl;
  cout << *(Word - 1) << endl;

  // Bug
}