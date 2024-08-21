/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:08:49 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/21 15:33:16 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*The accurate_sleep(data->time_to_die); in the handle_single_philosopher function serves a 
crucial purpose in simulating the dining philosophers problem for the edge case of having only one philosopher. 
Let's break down its meaning and purpose:

	Purpose:
	To simulate the passage of time until the philosopher "dies" of starvation.
	In the dining philosophers problem, each philosopher has a limited time to eat before they die.
	
	Meaning:
	data->time_to_die represents the maximum time a philosopher can go without eating before they die.
	The accurate_sleep function makes the thread wait for exactly this amount of time.
	
	Simulation accuracy:
	It ensures that the single philosopher case adheres to the same timing rules as the multi-philosopher scenarios.
	Without this sleep, the single philosopher case would end instantly, which wouldn't accurately represent the problem.
	
	Demonstrating the impossibility of the situation:
	With only one fork available, the philosopher can never eat (eating requires two forks).
	The sleep demonstrates that the philosopher will inevitably die in this scenario.
	
	Allowing for monitoring:
	This delay gives time for any monitoring threads or processes to detect and report the philosopher's death.
	
	Consistency with multi-threaded behavior:
	Even though there's only one philosopher, using accurate_sleep maintains consistency with how time is handled in the multi-threaded scenarios.
	
	Avoiding busy-waiting:
	Instead of continuously checking if the time has elapsed, accurate_sleep allows the program to yield control, saving CPU resources.
	
	In essence, this line simulates the philosopher sitting at the table, unable to eat, 
	until they "die" of starvation after the specified time. It's a crucial part of accurately 
	representing the dining philosophers problem, even in the simplified single-philosopher scenario. 
	Without this sleep, you would lose the time-based aspect of the simulation for this case, which is a key component of the problem.*/
void	handle_single_philosopher(t_philosopher *philo, t_data *data)
{
	print_status(data, philo->id, "has taken a fork");
	accurate_sleep(data->time_to_die);
}




/*Based on the analysis of the outputs and the behavior of the simulation, I recommend keeping both the Asymmetric Fork Acquisition (AFA) 
and the introduce_delay function. Here are the key reasons why:

1. Enhanced Deadlock Prevention:
   - AFA ensures that philosophers take forks in different orders, reducing the chance of circular waiting.
   - introduce_delay staggers the start times, further decreasing the likelihood of initial deadlocks.

2. Improved Fairness:
   - AFA helps balance fork acquisition among philosophers.
   - introduce_delay creates a more even distribution of start times, potentially leading to fairer resource allocation over time.

3. Reduced Contention:
   - AFA minimizes the time forks are held individually, reducing partial resource allocation.
   - introduce_delay spreads out initial fork requests, lowering initial resource contention.

4. More Stable Behavior:
   - The combination provides more predictable and consistent behavior across different runs.
   - This stability is beneficial for testing and can help in identifying other potential issues.

5. Robustness Across Scenarios:
   - Together, they make the simulation more resilient to various philosopher counts and timing parameters.
   - This robustness is valuable for handling edge cases and different configurations.

6. Efficient Resource Utilization:
   - AFA ensures philosophers take both forks quickly when available.
   - introduce_delay helps in creating a more efficient overall pattern of resource usage.

7. Closer to Real-world Scenarios:
   - The combination better simulates real-world concurrent systems where processes don't all start simultaneously and may have different resource acquisition patterns.

8. Educational Value:
   - Keeping both demonstrates multiple techniques for handling concurrency issues, which is valuable from an educational perspective.

9. Minimal Drawbacks:
   - Neither feature introduces significant overhead or complexity.
   - The benefits outweigh the minor additional code complexity.

10. Future-proofing:
    - This approach provides a solid foundation if you need to extend the project or handle more complex scenarios in the future.

By keeping both AFA and introduce_delay, you're implementing a more comprehensive solution to the dining philosophers problem. 
This approach not only solves the immediate requirements but also provides a more robust, fair, and realistic simulation of concurrent resource management.*/
void	introduce_delay(t_philosopher *philo, t_data *data)
//Function purpose: This function introduces a delay for some philosophers before they start their eating cycle.
{
	if (philo->id % 2 == 0)
	// Only applied to even philos
		usleep(data->time_to_eat * 500);
		/*The delay is set to half of the time_to_eat value (multiplied by 500 to convert from milliseconds to microseconds, as usleep takes microseconds).

		1. Time units:
		   - `data->time_to_eat` is typically given in milliseconds (ms).
		   - `usleep` function expects its argument in microseconds (μs).
		
		2. Conversion factor:
		   - 1 millisecond = 1000 microseconds
		   - However, the code uses 500 instead of 1000. Why?
		
		3. The factor of 500:
		   - This is actually implementing a half-duration sleep.
		   - 500 is half of 1000, so it's converting milliseconds to microseconds AND halving the duration in one step.
		
		4. How the conversion works:
		   - If `time_to_eat` is 200 ms:
		     200 * 500 = 100,000 μs
		   - This is equivalent to 100 ms (half of 200 ms)
		
		5. Why half duration:
		   - This introduces a delay that's half of the eating time.
		   - It staggers the philosophers without delaying them for a full eating cycle.
		
		To break it down step-by-step:
		1. Start with `time_to_eat` in milliseconds (e.g., 200 ms)
		2. Multiply by 1000 to convert to microseconds (200 * 1000 = 200,000 μs)
		3. Divide by 2 to get half the duration (200,000 / 2 = 100,000 μs)
		4. Steps 2 and 3 combined: 200 * (1000 / 2) = 200 * 500 = 100,000 μs
		
		So, multiplying by 500 is a shortcut to convert from milliseconds to microseconds and halve the duration in one operation.
		
		This approach introduces a delay that's significant enough to stagger the philosophers' start times, 
		but not so long that it drastically affects the overall simulation timing. It's a balanced way 
		to introduce desynchronization among the philosophers.*/





/* ╰(*°▽°*)╯ EXAMPLE: (●'◡'●)
Let's consider a scenario with 5 philosophers and the following parameters:
- time_to_die: 800 ms
- time_to_eat: 200 ms
- time_to_sleep: 200 ms

Without `introduce_delay`:
1. All philosophers start simultaneously.
2. Philosophers 1, 3, and 5 might immediately grab their left forks.
3. This could lead to an immediate deadlock or resource contention.

With `introduce_delay`:
1. Philosophers 1, 3, and 5 (odd IDs) start immediately.
2. Philosophers 2 and 4 (even IDs) wait for 100 ms (half of time_to_eat) before starting.
3. This staggered start allows for the following potential sequence:
  - t=0ms: Philosopher 1 and 3 might start eating
  - t=100ms: As 1 and 3 are still eating, 2 and 4 begin their cycle, but forks are occupied
  - t=200ms: 1 and 3 finish eating, allowing 5 to potentially start eating, while 2 and 4 can now access forks

Benefits:
1. Reduced initial contention: Not all philosophers compete for forks simultaneously at the start.
2. Deadlock prevention: The staggered start breaks potential circular wait conditions.
3. Improved resource utilization: It creates a natural "turn-taking" effect without explicit coordination.
4. Fairness: Over time, this initial offset tends to provide a more balanced distribution of eating opportunities.
╰(*°▽°*)╯ END OF EXAMPLE ☆*: .｡. o(≧▽≦)o .｡.:*☆*/
}

int	check_simulation_stop(t_data *data)
{
	int		stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
	/*
Key points and techniques:

1. Thread safety: The use of mutexes in `check_simulation_stop` ensures that reading the stop flag is thread-safe, preventing race conditions.
2. Modular design: The main loop is broken down into separate functions (think_and_take_forks, eat, release_forks_and_sleep), 
		which improves readability and maintainability.
3. Continuous loop with exit condition: The while loop continues indefinitely until an 
		external condition (simulation_stop) is set, allowing for dynamic control of the simulation duration.
*/
}

void	*philosopher_routine(void *arg)
{
	t_philosopher		*philo;
	/*philo is a pointer to the t_philosopher struct, which contains all the specific data for this philosopher.
	By declaring it as t_philosopher *philo, we create a properly typed pointer that we can use to access the 
		philosopher's data throughout the function.*/
	int					left_fork;
	int					right_fork;
	t_data				*data;

	philo = (t_philosopher *)arg;
	/*Purpose:
	This line is casting the void *arg parameter to a pointer of type t_philosopher *. It's a 
	crucial step in retrieving the specific data for this philosopher thread.
	
	Syntax breakdown:
	(t_philosopher *) is a cast operator. It tells the compiler to treat the following expression as a pointer to a t_philosopher struct.
	arg is the void pointer passed to the thread function. In C, void * is used as a generic pointer type that can point to any data type.
	
	Why it's needed:
	Thread functions in C (using pthread) must have the signature void *(*start_routine) (void *).
	This generic signature allows the same function to be used for different thread types, but 
	it means we need to cast the void * back to our specific data type inside the function.
	
	After this cast, philo points to the specific t_philosopher struct for this thread.
	We can then access the philosopher's data using philo->id, philo->data, etc.*/
	data = philo->data;
	/*Meaning: This line is assigning the data pointer from the philosopher struct to a local variable named data.
	data is indeed a local variable in the philosopher_routine function. 
	It's not initialized here, but rather assigned a value from philo->data.
	The actual initialization of the data structure likely happens in the main setup of the program, 
	probably in a function like init_data or run_simulation. The philosopher_routine is just creating a
	local reference to this already initialized structure.

	
	Purpose:
	philo->data is likely a pointer to the main t_data structure that contains all shared information about the simulation.
	By assigning it to a local variable data, we create a shortcut for easier access to this shared information throughout the function.
	This can make the code more readable and slightly more efficient, as we don't need 
	to dereference philo every time we want to access the shared data.*/
	left_fork = philo->id - 1;
	/*Meaning: This calculates the index of the left fork for this philosopher.
	
	Purpose:
	Philosophers are typically numbered from 1 to n, but array indices start at 0.
	By subtracting 1 from the philosopher's ID, we get the correct index for the left fork in the forks array.
	For example, philosopher 1 will use fork 0 as their left fork.*/
	right_fork = philo->id % data->num_philosophers;
	/*right_fork = philo->id % data->num_philosophers;

	Meaning: This calculates the index of the right fork for this philosopher.
	Purpose:
	
	The modulo operation (%) ensures that the right fork wraps around for the last philosopher.
	For most philosophers, this will simply be their ID (remember, ID starts at 1, so it's already offset correctly for array indexing).
	For the last philosopher (ID = num_philosophers), this will wrap around to 0, which is the first fork.
	This creates the circular table arrangement essential to the dining philosophers problem.
	
	Example:
	Let's say we have 5 philosophers:
	
	Philosopher 1: left_fork = 0, right_fork = 1
	Philosopher 2: left_fork = 1, right_fork = 2
	Philosopher 3: left_fork = 2, right_fork = 3
	Philosopher 4: left_fork = 3, right_fork = 4
	Philosopher 5: left_fork = 4, right_fork = 0 (wraps around) <-------*/
	if (data->num_philosophers == 1)
	{
		handle_single_philosopher(philo, data);
		return (NULL);
	}
	introduce_delay(philo, data);
	philo->last_meal_time = get_current_time();
	while (check_simulation_stop(data) == 0)
	// this loop while continue until the return value is 0
	{
		think_and_take_forks(philo, data, left_fork, right_fork);
		eat(philo, data);
		release_forks_and_sleep(philo, data, left_fork, right_fork);
	}
	return (NULL);
}
