# Philosophers

<p align="center">
  <img src="https://github.com/mcombeau/mcombeau/blob/main/42_badges/philosopherse.png" alt="Philosophers 42 project badge"/>
</p>

## Project Overview

The Philosophers project is a simulation of the classic dining philosophers problem, which is a common example of a synchronization problem in computer science. The goal of this project is to implement a solution that allows multiple philosophers to eat from shared resources (forks) while avoiding deadlock and ensuring that all philosophers can eat in a timely manner.

## Features

- **Multithreading**: Each philosopher is represented as a separate thread, allowing them to think, eat, and sleep concurrently.
- **Mutexes**: The project utilizes mutexes to manage access to shared resources (forks) and to ensure that philosophers do not interfere with each other while eating.
- **Command-Line Arguments**: The program accepts command-line arguments to configure the number of philosophers, the time each philosopher takes to eat, sleep, and the time to die.
- **Error Handling**: The program includes robust error handling for invalid input and memory allocation failures.

## How It Works

1. **Initialization**: The program initializes the necessary data structures and mutexes based on the command-line arguments provided.
2. **Philosopher Threads**: Each philosopher is created as a thread that executes a routine where they alternate between thinking, taking forks, eating, and sleeping.
3. **Death Clock**: A separate thread monitors the state of the philosophers to determine if any have "died" (i.e., if they have not eaten within a specified time).
4. **Cleanup**: Upon completion of the simulation, all allocated resources are freed, and mutexes are destroyed to prevent memory leaks.

## Usage

To compile the project, use the provided Makefile:

```bash
make
```

To run the simulation, execute the compiled program with the required arguments:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep>
```

- `number_of_philosophers`: The number of philosophers participating in the simulation.
- `time_to_die`: The time in milliseconds after which a philosopher is considered dead if they have not eaten.
- `time_to_eat`: The time in milliseconds that a philosopher takes to eat.
- `time_to_sleep`: The time in milliseconds that a philosopher sleeps after eating.
- `meals_to_eat` (optional): The number of meals each philosopher must eat before the simulation ends.

## Example

```bash
./philo 5 800 200 200 3
```

This command starts a simulation with 5 philosophers, where each philosopher must eat 3 meals, has 800 milliseconds to live without eating, takes 200 milliseconds to eat, and sleeps for 200 milliseconds after eating.

## Conclusion

The Philosophers project serves as an educational tool for understanding synchronization issues in concurrent programming. It demonstrates the use of threads, mutexes, and proper resource management in a multithreaded environment.

---
Made by pmolzer: pmolzer@student.42berlin.de | LinkedIn: [pmolzer](https://www.linkedin.com/in/peter-moelzer//) 
