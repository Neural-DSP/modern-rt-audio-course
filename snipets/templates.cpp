#include <iostream>
#include <cmath>

// For std::array
#include <array>

// For std::pair
#include <utility>

// Template allows to use type as arguments for a class
// Other than type, template also allow compile-time
// integer constants to be defined
template<typename Key, typename Value, unsigned int Size>
class PairQueue
{
public:
    PairQueue() = default;

    // Queue need an extra slot - lazy algorithm
    static const unsigned int ArraySize { Size + 1 };

    // Type alias for pair type
    // https://en.cppreference.com/w/cpp/utility/pair
    using Pair = std::pair<Key, Value>;

    // Type alias for array type
    // https://en.cppreference.com/w/cpp/container/array
    using Queue = std::array<Pair, ArraySize>;

    // No copy and move
    PairQueue(const PairQueue&) = delete;
    PairQueue(PairQueue&&) = delete;
    const PairQueue& operator=(const PairQueue&) = delete;
    const PairQueue& operator=(PairQueue&&) = delete;

    // Clear queue by reseting indices
    void clear_queue()
    {
        write_index = read_index = 0u;
    }

    // Get used slots by calculating the distante
    // between write and read indices
    unsigned int get_used() const noexcept
    {
        // We can only do it this simple because of the extra slot
        return (write_index + ArraySize - read_index) % ArraySize;
    }

    // Push pair to queue
    // Returns false if the queue is full
    bool push_pair(const Pair& pair)
    {
        if (get_used() < Size)
        {
            queue[write_index] = pair;
            ++write_index; write_index %= ArraySize;
            return true;
        }
        return false;
    }

    // Pop pair from the queue
    bool pop_pair(Pair& pair)
    {
        if (get_used() > 0)
        {
            pair = queue[read_index];
            ++read_index; read_index %= ArraySize;
            return true;
        }
        return false;
    }

private:
    Queue queue;
    unsigned int write_index { 0 };
    unsigned int read_index { 0 };
};

int main()
{
    static const unsigned int Size { 8 };

    // Instance a queue
    PairQueue<unsigned int, float, Size> pair_queue;

    // Fill with some pairs
    bool success_push { true };
    for (unsigned int i = 0; i < (Size /*+1*/); ++i)
    {
        std::pair<unsigned int, float> pair { std::make_pair(i, std::sqrt(static_cast<float>(i))) };
        if ((success_push = pair_queue.push_pair(pair)))
            std::cout << "Push pair: " << pair.first << "/" << pair.second << std::endl;
        else
            std::cout << "Failed push!" << std::endl;
    }

    if (!success_push)
        std::cout << "Queue is full!" << std::endl;
    else
        std::cout << "Queue has " << pair_queue.get_used() << " used slots." << std::endl;

    bool success_pop { true };
    for (unsigned int i = 0; i < (Size /*+1*/); ++i)
    {
        std::pair<unsigned int, float> pair;
        if ((success_pop = pair_queue.pop_pair(pair)))
            std::cout << "Pop pair: " << pair.first << "/" << pair.second << std::endl;
        else
            std::cout << "Failed pop!" << std::endl;
    }

    return 0;
}
