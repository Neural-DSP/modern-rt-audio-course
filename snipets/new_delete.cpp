#include <iostream>

// For std::sqrt
#include <cmath>

class Vector3
{
public:
    // Default ctor, all values initialised to 0.0
    Vector3() = default;

    // Main ctor, allows specifying components
    Vector3(float _i, float _j, float _k) :
        i { _i }, j { _j }, k { _k }
    {}

    // Copy ctor
    Vector3(const Vector3& other) :
        i { other.i }, j { other.j }, k { other.k }
    {}

    // Copy asign
    const Vector3 operator=(const Vector3& rhs)
    {
        i = rhs.i;
        j = rhs.j;
        k = rhs.k;

        return *this;
    }

    // Getters
    float get_i() const noexcept { return i; }
    float get_j() const noexcept { return j; }
    float get_k() const noexcept { return k; }

    // Setters
    void set_i(float _i) { i = _i; }
    void set_j(float _j) { j = _j; }
    void set_k(float _k) { k = _k; }
    void set_vector(float _i, float _j, float _k) { i = _i; j = _j; k = _k; };

    // Get the modulo of the vector
    float get_modulo() const noexcept { return std::sqrt(i*i + j*j + k*k); }

    // Print function for ilustration
    void print() const
    {
        std::cout << "i: " << i << "; j: " << j << "; k: " << k << std::endl;
    }

private:
    float i { 0.f };
    float j { 0.f };
    float k { 0.f };
};

int main()
{
    // Empty pointer which can be asigned to a memory location
    Vector3* vector_ptr { nullptr };

    // Dynamically allocate (heap) a single vector with 'new'
    vector_ptr = new Vector3(1.f, 2.f, 3.f);

    // Print the vector
    std::cout << "Print single vector" << std::endl;
    std::cout << "Modulo: " << vector_ptr->get_modulo() << std::endl;
    vector_ptr->print();
    std::cout << std::endl; // Extra new line for readbility

    // Free the memory with 'delete'
    delete vector_ptr;

    // Recomended to get rid of the old memory address
    // which is now invalid
    vector_ptr = nullptr;

    // Allocates an array of Vector3 and return
    // a point with the memory address of the first
    const unsigned int num_of_vectors { 10 };
    vector_ptr = new Vector3[num_of_vectors];

    // Fill the vector with some values
    for (unsigned int i = 0; i < num_of_vectors; ++i)
        vector_ptr[i] = Vector3(static_cast<float>(i+1), static_cast<float>(i+2), static_cast<float>(i+3));

    std::cout << "Print vector array" << std::endl;
    for (unsigned int i = 0; i < num_of_vectors; ++i)
    {
        std::cout << "Vector3[" << i << "] Modulo: " << vector_ptr[i].get_modulo() << std::endl;
        vector_ptr[i].print();
        std::cout << std::endl;
    }

    // Free the memory for the entire array with 'delete []'
    delete [] vector_ptr;

    return 0;
}
