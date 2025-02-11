#include <iostream>
#include <memory>
#include <vector>

void cpp11() {
    // Smart Pointers, make_unique is from c++14
    auto uptr = std::make_unique<int>(42);  // unique_ptr
    auto sptr = std::make_shared<int>(100); // shared_ptr

    // Lambda Function
    auto print = [](const auto& value) {
        std::cout << "Value: " << value << std::endl;
    };
    
    print(*uptr);
    print(*sptr);

    // Move Semantics
    std::vector<std::unique_ptr<int>> vec;
    vec.push_back(std::move(uptr)); // Moving unique_ptr into vector
    
    // Auto Keyword
    for (const auto& ptr : vec) {
        print(*ptr);
    }

    // Lambda capturing a variable
    int multiplier = 2;
    auto multiply = [multiplier](int num) {
        return num * multiplier;
    };
    
    std::cout << "Result of multiply(5): " << multiply(5) << std::endl;
}