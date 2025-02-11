#include <iostream>
#include <TH1F.h>

void check_memory() 
//int main() 
{

    for (int i = 0; i < 1000; ++i)
        TH1F *hist = new TH1F(Form("hist%i",i), "Histogram with Statistical Error Bars;X-axis;Y-axis", 50, 0, 50);
    //return 0;
}

/*

g++ -o myMacro check_memory.C `root-config --cflags --libs`
valgrind --leak-check=full ./myMacro

valgrind --tool=memcheck --leak-check=yes --log-file=val.log root -l -b -q "check_memory.C+" 

*/