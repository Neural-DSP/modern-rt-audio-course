#include <iostream>

int main()
{
    unsigned int index { 0 };

    static unsigned int signal_size { 10 };
    static unsigned int buffer_size { 4 };
    for (unsigned int i = 0; i < signal_size; ++i)
        std::cout << (index = (index + 1) % buffer_size) << std::endl;
    return 42;
}
