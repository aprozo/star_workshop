#include <iostream>

class Counter {
public:
    static int count;  // Shared across all instances

    Counter() { count++; }

    static void showCount() {  // Static function (no `this` pointer)
        std::cout << "Count: " << count << std::endl;
    }
};


int Counter::count = 0; // Definition of static member


int raise_counter(){
    static int num = 10; // Static variable (persists across function calls)
    return ++num;
}

void static_example() {
    Counter c1, c2;
    Counter::showCount(); // Outputs: Count: 2

    std::cout << "Static variable: " << raise_counter() << std::endl;
    std::cout << "Static variable: " << raise_counter() << std::endl;
    std::cout << "Static variable: " << raise_counter() << std::endl;
}
