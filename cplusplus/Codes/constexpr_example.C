#include <iostream>

constexpr int square(int x) {
    return x * x;
}

void constexpr_example() {
    constexpr int result = square(5); 
    int nonconstexpr_result = square(5);
    std::cout << "Square: " << result <<" vs "<< nonconstexpr_result  << std::endl;
}
