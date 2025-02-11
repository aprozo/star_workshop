#include <iostream>

class Object {
protected:
    int dimension;
public:
    Object(int d) : dimension (d*d*d) {}; //explicit

    inline virtual void setDimension(int d) { dimension = d*d*d;}
    
    friend void printDimension(const Object& o); // Friend function

    virtual void display() final {} // Cannot be overridden in derived classes
};

class Box : public Object  {
public:
    using Object::Object; // Inherits Base's constructor
    inline void setDimension(int d) override { dimension = d;}
    // void display() {} // Error
};

void printDimension(const Object& o) {
    std::cout << "Width: " << o.dimension << std::endl;
}

void class_example() {
    Box explicitBox(10); // Explicit conversion from int to Box
    printDimension(explicitBox);  // Can access private `width`


    Box implicitBox = 10; // Implicit conversion from int to Box
    implicitBox.setDimension(10);
    printDimension(implicitBox);  // Can access private `width`
}
