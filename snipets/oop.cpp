// Some OOP features in C++

// Math functions
#include <cmath>

// Included for debug logging
#include <iostream>

// Handy macro for logging
#define DBG(str) { std::cout << str << std::endl; }

// Define Polygon class
class Polygon
{
// The class definition has "private" scope by default
public:
    // Constructor
    Polygon(unsigned int _num_of_edges)
      : num_of_edges { _num_of_edges } // inline member initialisaiton
    {
        DBG("Polygon::Polygon -> num_of_edges: " << num_of_edges)
    }

    // Interface methods

    unsigned int get_num_of_edges() const
    {
        DBG("Polygon::get_num_of_edges")
        return num_of_edges;
    }

    // Abtract interface methods
    // Thos are meant to be implemented by the derived classes

    virtual float get_area() const = 0; // The =0 means that the method
                                        // is not implemented.

    // Implemented version
    // virtual float get_area() const
    // {
    //     DBG("Polygon::get_area")
    //     return 0.f;
    // }

    // Destructor
    // When a class is intended to be inherited
    // making the base class dtor as virtual guaratees
    // that the derived class dtor will be called.
    virtual ~Polygon() { DBG("virtual Polygon::~Polygon") }

    // Non-virtual dtor
    // ~Polygon() { DBG("Polygon::~Polygon") }

private:
    unsigned int num_of_edges;
};

// Derived class
class Triangle : public Polygon
{
public:
    Triangle(float _edge_length)
      : Polygon(3), // base-class initialisation
        edge_length { _edge_length }
    {
        DBG("Triangle::Triangle -> edge_length: " << edge_length)
    }

    ~Triangle()
    {
        DBG("Triangle::~Triangle()")
    }

    // Override of the abstract interface
    float get_area() const override
    {
        DBG("Triangle::get_area")
        return std::sqrt(3.f) * edge_length * edge_length / 4.f;
    }

private:
    float edge_length;
};

// Uses Polygon base class
void print_polygon_info(const Polygon& poly)
{
    unsigned int num_of_edges = poly.get_num_of_edges();
    float area = poly.get_area();
    DBG("::print_polygon_info() -> num_of_edges: " << num_of_edges << "; area: " << area << ";")
}

int main()
{
    // Try to instantiate Polygon
    // Polygon poly(4); // Error: Polygon has virtaul non-implemented methods

    { // Open scope - call ctor
        DBG("Instance Triangle object: tri_0")
        Triangle tri_0(3.5f); // Instance Triangle - Ok!
        float area = tri_0.get_area();
        DBG("tri_0.get_area() -> " << area)
    } // End of scope - call dtor

    DBG("") // Print some empty line to make logs more readable

    unsigned int num_of_polygons { 3 };
    Triangle polygons[3] = { Triangle(3.1f), Triangle(4.2f), Triangle(5.3f) };

    DBG("") // Print some empty line to make logs more readable

    for (unsigned int n = 0; n < num_of_polygons; ++n)
    {
        DBG("Use Polygon base class! No knoledge about derived class triangle.")
        print_polygon_info(polygons[n]);
        DBG("") // Print some empty line to make logs more readable
    }

    return 42;
}
