#include <iostream>
#include "TMath.h"
using namespace std;

// Function to process different conditions
void particles(Bool_t pion = kTRUE, Bool_t kaon = kFALSE)
{
    cout << "Running task with parameters:" << endl;
    cout << "pion = " << pion << ", kaon = " << kaon << endl;

    // Logic depending on pion and kaon values
    if (pion)
        cout << "pion is TRUE, kaon is FALSE" << endl;
    else if (kaon)
        cout << "pion is FALSE, kaon is TRUE" << endl;
        
}