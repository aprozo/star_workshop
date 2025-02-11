#include <iostream>
#include <memory>
#include <TH1F.h>

class Example {
public:
    Example(int val) : value(val) {
        std::cout << "Example constructed with value: " << value << std::endl;
    }
    ~Example() {
        std::cout << "Example destructed with value: " << value << std::endl;
    }
    void show() const {
        std::cout << "Value: " << value << std::endl;
    }
private:
    int value;
};

void pointers() 
{

    // Using unique_ptr
    //std::unique_ptr<Example> uPtr = std::make_unique<Example>(10); // c++14+
    std::unique_ptr<Example> uPtr(new Example(10));
    uPtr->show();
    
    // Transferring ownership using move semantics
    std::unique_ptr<Example> uPtr2 = std::move(uPtr);
    if (!uPtr) {
        std::cout << "uPtr is now nullptr" << std::endl;
    }
    uPtr2->show();
    
    // Using shared_ptr
    //std::shared_ptr<Example> sPtr1 = std::make_shared<Example>(20);
    std::shared_ptr<Example> sPtr1(new Example(20));
    std::shared_ptr<Example> sPtr2 = sPtr1; // Shared ownership
    
    sPtr1->show();
    sPtr2->show();
    
    std::cout << "Shared pointer use count: " << sPtr1.use_count() << std::endl;
}
