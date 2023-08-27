#include <iostream>
#include <vector>

// Include for std::function
// https://en.cppreference.com/w/cpp/utility/functional/function
#include <functional>

class TaskPipeline
{
public:
    // Type alias for the task function signature
    using Task = std::function<float(float)>;

    TaskPipeline() = default;
    ~TaskPipeline() {}

    // No copy/move
    TaskPipeline(const TaskPipeline&) = delete;
    TaskPipeline(TaskPipeline&&) = delete;
    const TaskPipeline& operator=(const TaskPipeline&) = delete;
    const TaskPipeline& operator=(TaskPipeline&&) = delete;

    // Add a task to the pipeline
    void add_task(Task task)
    {
        // Check if not null and add to pipeline
        if (task)
            pipeline.push_back(task);
    }

    // Process the task pipeline
    float process_tasks(float input)
    {
        // Cycle thru the pipeline executing the task
        // Feeding the output of the previous to the next task
        for (auto task : pipeline)
        {
            input = task(input);
        }
        return input;
    }

private:
    // Vector to hold the tasks of the pipeline
    std::vector<Task> pipeline;
};

int main()
{
    TaskPipeline task_pipeline;

    // Lambda expressions can be directly assigned
    // to a std::function as long as the signature matches
    task_pipeline.add_task([] (float input)
    {
        const float result { input + 1.f };
        std::cout << "Task 1: add 1.0; Result: " << result << std::endl;
        return result;
    });

    task_pipeline.add_task([] (float input)
    {
        const float result { input * 2.5f };
        std::cout << "Task 2: multiply by 2.5; Result: " << result << std::endl;
        return result;
    });

    task_pipeline.add_task([] (float input)
    {
        const float result { input / 1.25f };
        std::cout << "Task 3: divide by 1.25; Result: " << result << std::endl;
        return result;
    });

    // Process the pipeline
    const float result { task_pipeline.process_tasks(4.7f) };
    std::cout << "Final result: " << result << std::endl;

    return 0;
}