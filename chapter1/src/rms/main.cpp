#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Define the number of tasks
#define NUM_TASKS 3

// Define task function pointer type
typedef void (*task_function_t)(void);

// Define structure for each task
typedef struct {
    unsigned int intervalSec;    // Task interval in seconds
    task_function_t func;        // Pointer to the task function
} Task;

// Define an array of tasks
Task tasks[NUM_TASKS];

// Task 0 to 2 function
void task0() {
    cout << "Task 0 executed" << endl;
}

void task1() {
    cout << "Task 1 executed" << endl;
}

void task2() {
    static bool condition = false;
    cout << "Task 2 executed" << endl;
    if (condition) {
        tasks[2].func = nullptr;
    }
}

// Simple time-triggered scheduler
void schedulerLoop(int times) {
    unsigned int currentTask = 0;
    unsigned int currentTime = 0;

    while (times > 0) {
        // Update current time
        currentTime += tasks[currentTask].intervalSec;

        // Execute the current task
        if (tasks[currentTask].func) {
            tasks[currentTask].func();
            // Sleep for the interval
            std::this_thread::sleep_for(std::chrono::seconds(tasks[currentTask].intervalSec));
        }
    
        // Determine next task
        currentTask = (currentTask + 1) % NUM_TASKS;

        times--;
    }
}

int main(int argc, char *argv[]) {
    cout << "Bare metal task manager started" << endl;
    
    // Define your tasks and their intervals
    tasks[0].intervalSec = 1; // Task 0 runs every 1 second
    tasks[0].func = &task0;
    tasks[1].intervalSec = 2; // Task 1 runs every 2 seconds
    tasks[1].func = &task1;
    tasks[2].intervalSec = 3; // Task 2 runs every 3 seconds
    tasks[2].func = &task2;

    // Start the scheduler loop for N loops
    int loops = 10;
    cout << "Run " << 10 << " loops of the scheduler" << endl;
    schedulerLoop(10);
    
    cout << "Bare metal task manager finished" << endl;
    return 0;
}