#include <iostream>

// Include smart pointers
#include <memory>

class MyClass
{
public:
    MyClass()
    {
        std::cout << "MyClass: Default Ctor" << std::endl;
    }

    ~MyClass()
    {
        std::cout << "MyClass: Dtor" << std::endl;
    }
};

int main()
{
    {
        // A unique_ptr instanced inside a scope and allocate it
        std::cout << "Instanciate 'MyClass' in unique_ptr" << std::endl;
        std::unique_ptr<MyClass> up { new MyClass };
        // Should be destroyed at the end of the scope
    }
    std::cout << "Should be destroyed before here" << std::endl;
    std::cout << std::endl; // extra new line for readability

    // Instanciate a shared_ptr without allocating
    std::shared_ptr<MyClass> sp0;
    {
        std::cout << "Instanciate 'MyClass' in a shared_ptr" << std::endl;
        std::shared_ptr<MyClass> sp1 { new MyClass };

        // Asign to another shared_ptr
        sp0 = sp1;
    }
    std::cout << "Should NOT be destroyed before here" << std::endl;

    // Gets destroyed once the 'main' function returns
    return 0;
}
