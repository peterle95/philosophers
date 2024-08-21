/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:54:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/19 14:00:41 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*This code implements the classic Dining Philosophers problem using threads in C. 
Here's an overall description of the main steps and components:

1. Input Validation and Initialization:
   - Validates command-line arguments (number of philosophers, time to die, time to eat, time 
   to sleep, and optionally, number of times each philosopher must eat).
   - Initializes data structures, including philosophers and forks (represented by mutexes).

2. Thread Creation:
   - Creates a thread for each philosopher.
   - Creates a monitor thread to check for deaths or if all philosophers have eaten enough.

3. Philosopher Behavior (in each philosopher thread):
   - Philosophers alternate between thinking, eating, and sleeping.
   - To eat, a philosopher must acquire two forks (mutexes) - one to their left and one to their right.
   - After eating, the philosopher releases the forks and goes to sleep.

4. Fork Management:
   - Forks are represented by mutexes.
   - Philosophers must lock both their left and right fork mutexes before eating.
   - Forks are released (mutexes unlocked) after eating.

5. Death Monitoring:
   - The monitor thread regularly checks if any philosopher has starved (exceeded time_to_die since their last meal).
   - If a philosopher dies, the simulation is stopped.

6. Meal Counting:
   - If specified, the program tracks how many times each philosopher has eaten.
   - The simulation can end when all philosophers have eaten a certain number of times.

7. Time Management:
   - Uses gettimeofday() for precise timing.
   - Implements a custom sleep function (accurate_sleep) for more precise delays.

8. Concurrency Control:
   - Uses mutexes to prevent race conditions when philosophers are taking forks, eating, or when the program is printing status updates.

9. Simulation End:
   - The simulation ends when a philosopher dies or when all philosophers have eaten enough times (if specified).
   - All threads are joined, and resources (mutexes, allocated memory) are cleaned up.

10. Special Cases:
    - Handles the case of a single philosopher differently, as it's impossible for them to eat (they can only hold one fork).
    - Introduces a small delay for even-numbered philosophers to reduce the chance of deadlock.

This implementation uses threads to simulate concurrent philosopher actions, mutexes to 
represent forks and ensure exclusive access to shared resources, and careful timing to manage
the lifecycle of philosophers and detect starvation.


☆*: .｡ o(≧▽≦)o .｡.:*☆ OVERVIEW ☆*: .｡. o(≧▽≦)o .｡.:*☆

I'll provide a comprehensive overview of the code based on the aspects you've outlined.

### Overall Structure and Purpose

This program implements the Dining Philosophers problem, a classic synchronization and concurrency problem in computer science. 
The main components include:

1. Main program structure (`t_program`)
2. Philosopher structure (`t_philosopher`)
3. Initialization functions
4. Thread creation and management
5. Philosopher routines
6. Monitoring and death checking
7. Utility functions

The primary goal is to simulate philosophers sitting at a table, alternating between thinking, eating, and sleeping, while sharing forks (resources) with their neighbors.

### Key Algorithms and Data Structures

- Mutex locks for forks and writing
- Circular arrangement of philosophers and forks
- Time management using `gettimeofday()`
- Thread-based concurrency for philosophers and monitoring

### Control Flow

1. Program initializes data structures and mutexes
2. Creates threads for each philosopher and a monitor thread
3. Philosophers cycle through thinking, eating, and sleeping
4. Monitor thread checks for deaths or completion conditions
5. Program terminates when all philosophers have eaten enough or one dies

### Data Management

- Philosopher states and meal counts stored in `t_philosopher` structures
- Program-wide data (timings, mutex locks) stored in `t_program` structure
- Shared resources (forks) managed using mutex locks

### Error Handling

- Input validation for command-line arguments
- Error checking for memory allocation, thread creation, and mutex operations
- Graceful cleanup in case of errors during initialization or execution

### External Dependencies

- Standard C libraries: `stdio.h`, `string.h`, `stdlib.h`
- POSIX thread library: `pthread.h`
- System-specific libraries: `sys/time.h`, `unistd.h`

### Potential Bottlenecks

- Frequent time checks in the monitor thread
- Potential for deadlock if not properly managed (though the even/odd fork picking strategy helps mitigate this)

### Code Organization

- Well-structured with separate files for different functionalities
- Consistent naming conventions (e.g., `snake_case` for functions)
- Good use of header files and proper include guards

### Unique Solutions

- Introducing a delay for even-numbered philosophers to reduce contention
- Using a separate monitor thread for death checking

### Areas for Improvement

- Consider using condition variables for more efficient waiting
- Implement more robust error reporting and logging

### Threading and Concurrency

- Uses POSIX threads for parallel execution of philosophers
- Separate monitor thread for checking termination conditions
- Mutex locks for synchronizing access to shared resources (forks and output)

### Main Challenges

- Proper synchronization to avoid deadlocks and race conditions
- Accurate timing and death detection
- Balancing performance with correct behavior

### Documentation

- Good inline comments explaining key functions and algorithms
- Could benefit from more high-level documentation explaining the overall design and rationale

### Summary of Crucial Points

1. Understanding of the Dining Philosophers problem and its challenges
2. Proper use of mutex locks for synchronization
3. Accurate time management and death detection
4. Thread creation and management for philosophers and monitoring
5. Input validation and error handling throughout the program

To work with or maintain this code effectively, one would need a solid grasp of multithreading concepts, 
POSIX thread API, and the specifics of the Dining Philosophers problem. 
Additionally, understanding the timing mechanisms and how they relate to the problem constraints is crucial.*/

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h> // printf is allowed
# include <string.h> // memset is allowed
# include <pthread.h>
# include <stdlib.h> // malloc & free
# include <sys/time.h> // gettimeofday
# include <unistd.h> // usleep & write
# include <limits.h>
# include <stdbool.h>

typedef struct s_philosopher {
    int id;
    int times_eaten;
    long long last_meal_time;
    pthread_t thread_philo; // A thread identifier for the thread that will represent this philosopher.
    struct s_data *data; // A pointer to a s_data structure (defined later) that contains shared data for all philosophers.
} t_philosopher;

typedef struct s_data {
    int num_philosophers;
    long long time_to_die;
    long long time_to_eat;
    long long time_to_sleep;
    /*The use of different integer types (`long` and `long long`) in this code is related to precision requirements and system compatibility. 
    Let's break this down:

1. `long` is used for:
   - Time values that represent durations (e.g., `time_to_die`, `time_to_eat`, `time_to_sleep`)
   - Function return values for time-related functions (e.g., `get_current_time()`)

2. `long long` is used for:
   - Timestamp values (e.g., `start_time`, `last_meal_time`)
   - Time difference calculations

The reason for this distinction is likely twofold:

1. Precision: `long long` provides a larger range of values, which is particularly useful for 
timestamps that might need to represent a very large number of milliseconds since an epoch.

2. Compatibility: On some systems, `long` might be 32-bit while `long long` is guaranteed to be at 
least 64-bit. Using `long long` for timestamps ensures that the program can run for extended periods without overflow.

However, it's worth noting that this mixing of types can lead to potential issues:

- There's a risk of implicit type conversion when comparing or assigning between `long` and `long long` values.
- It can make the code less consistent and potentially confusing.

For better consistency and to avoid potential overflow issues, it might be preferable to use `long long` uniformly for all time-related values. Alternatively, 
defining a custom type (e.g., `typedef long long t_time;`) could improve code clarity and make it easier to change the underlying type if needed in the future.*/
    int meals_to_eat;
    long long start_time; // The timestamp when the simulation starts.
    int simulation_stop; // A flag to indicate when the simulation should stop.
    pthread_mutex_t stop_mutex; // A mutex to protect access to the simulation_stop flag.
    pthread_mutex_t *forks; // An array of mutexes representing the forks that philosophers use to eat.
    
            /*A visual representation of pthread_mutex_t *forks; would be an array of mutexes, where each mutex represents 
            a fork in the dining philosophers problem. Here's a simple ASCII art representation:
                
                Philosophers:   P1    P2    P3    P4    P5
                                 |     |     |     |     |
                                 v     v     v     v     v
                Forks:         [M1]--[M2]--[M3]--[M4]--[M5]
                                 ^                       ^
                                 |                       |
                                 -------------------------
            
            In this visualization:
            P1, P2, P3, P4, P5 represent the philosophers
            M1, M2, M3, M4, M5 represent the mutexes (forks)
            Each mutex is shared between two adjacent philosophers
            The line connecting M5 back to M1 represents the circular nature of the table
            
            Each [M] box represents a pthread_mutex_t object, and the forks pointer 
            would point to the first element of this array of mutexes.
            In memory, it would look something like this:
                
                Memory:  [M1][M2][M3][M4][M5]
                          ^
                          |
                forks ----+
            
            Here, forks is a pointer to the first mutex in a contiguous block of memory containing all the mutex objects.
            This arrangement allows each philosopher to attempt to lock the mutexes (pick up the forks) 
            to their left and right when they want to eat. The use of mutexes ensures that no two adjacent 
            philosophers can use the same fork simultaneously, which is crucial for preventing deadlocks in the dining philosophers problem.*/
    pthread_mutex_t write_lock; // A mutex to ensure that only one philosopher writes to the console at a time.
    
    t_philosopher *philosophers;
} t_data;

// Utils
long	get_current_time(void);
void	print_status(t_data *data, int id, char *status);
long long	ft_atoll(const char *str);
void	set_simulation_stop(t_data *data);

// Cleanup
void	cleanup(t_data *data);

// Check
int		check_philosopher_deaths(t_data *data);
int		check_all_philosophers_ate_enough(t_data *data);

// Threads
int		create_and_run_threads(t_data *data);

// Init
int		init_data(t_data *data, int argc, char **argv);

// Philosopher
void	*philosopher_routine(void *arg);

// Death Clock
bool	is_philosopher_dead(t_philosopher *philo, t_data *data);
void	*death_clock_routine(void *arg);
bool	all_philosophers_ate_enough(t_data *data);

// Validate Input
int		validate_input(int argc, char **argv);

// Memory Allocation
int		allocate_memory(t_data *data);

// Eat Think And Sleep
void	accurate_sleep(long long time_in_ms);
void	release_forks_and_sleep(t_philosopher *philo, t_data *data,
			int left_fork, int right_fork);
void	think_and_take_forks(t_philosopher *philo, t_data *data,
			int left_fork, int right_fork);
void	eat(t_philosopher *philo, t_data *data);

// Mutexes
int initialize_stop_mutex(t_data *data);
int initialize_write_lock(t_data *data);
void destroy_fork_mutexes(t_data *data, int num_to_destroy);
int initialize_fork_mutexes(t_data *data);

#endif