#include <iostream>

class Test {
    mutable int count = 0;  // Can change even in const function
public:
    void increment() const {
        count++;
        std::cout << "Count: " << count << std::endl;
    }
};

void mutable_example() {
    Test obj;
    obj.increment();  // Allowed even though the function is const
}
