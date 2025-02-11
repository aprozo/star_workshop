#include <iostream>
#include <stdexcept>

// Testovací funkce
int add(int a, int b) {
    return a + b;
}

// Makro pro testování očekávané hodnoty
#define ASSERT_EQUAL(expected, actual) \
    if ((expected) != (actual)) { \
        std::cerr << "Test failed: " << __FILE__ << ":" << __LINE__ \
                  << " Expected " << (expected) << ", but got " << (actual) << std::endl; \
        return false; \
    }

// Jednotlivé testy
bool testAdditionPositiveNumbers() {
    ASSERT_EQUAL(5, add(2, 3));
    return true;
}

bool testAdditionNegativeNumbers() {
    ASSERT_EQUAL(-5, add(-2, -3));
    return true;
}

// Spuštění testů
void test() {
    int failedTests = 0;

    if (!testAdditionPositiveNumbers()) {
        std::cerr << "testAdditionPositiveNumbers failed!" << std::endl;
        failedTests++;
    }
    
    if (!testAdditionNegativeNumbers()) {
        std::cerr << "testAdditionNegativeNumbers failed!" << std::endl;
        failedTests++;
    }

    if (failedTests == 0) {
        std::cout << "All tests passed successfully!" << std::endl;
    } else {
        std::cerr << failedTests << " test(s) failed!" << std::endl;
    }
}
