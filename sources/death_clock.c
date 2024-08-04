/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   death_clock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:15:29 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/01 14:11:02 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	kill_single_philosopher(t_data *data)
{
	accurate_sleep(data->time_to_die);
	printf("%lld 1 died\n", data->time_to_die);
	data->simulation_stop = 1;
}

bool	is_philosopher_dead(t_philosopher *philo, t_data *data)
{
	long	current_time;
	// Stores the current timestamp.
	long	time_since_last_meal;
	long	death_time;
	// Used to store the exact time of death if the philosopher dies.

	// check with debugg info, when philo threads start and when death_clock start --> delay function
	// (necessary? Because the time_since_last_meal calculates the difference of current_time and the last meal time)
	current_time = get_current_time();
	time_since_last_meal = current_time - philo->last_meal_time;
	// Calculates the time elapsed since the philosopher's last meal.
	if (time_since_last_meal > data->time_to_die)
	/*Checks if the philosopher has gone too long without eating.*/
	{
		pthread_mutex_lock(&data->stop_mutex);
		/*Locks the stop mutex to safely check and modify the simulation stop flag.*/
		if (data->simulation_stop == 0)
		/*The line if (data->simulation_stop == 0) is checking whether the simulation has already been stopped. 
		This condition is true when data->simulation_stop is 0.
		Let's imagine a scenario with 5 philosophers:

		Initially, data->simulation_stop is set to 0, meaning the simulation is running.
		Now, let's say Philosopher 3 and Philosopher 5 both exceed their time limit without eating at almost the same time.
		The is_philosopher_dead function gets called for Philosopher 3 first:

		It checks if (data->simulation_stop == 0)
		Since data->simulation_stop is 0, data->simulation_stop is true
		The function enters the if block
		It sets data->simulation_stop = 1
		It prints the death message for Philosopher 3
		It returns true

		Almost immediately after, is_philosopher_dead gets called for Philosopher 5:

		It again checks if (data->simulation_stop == 0)
		But now, data->simulation_stop is 1
		So data->simulation_stop is false
		The function does not enter the if block
		It returns false without printing a death message

		This check ensures that only the first detected death is reported and processed, even if multiple philosophers "die" at nearly the same time. 
		It's like a race where only the first to cross the finish line gets declared the winner, even if others finish a split second later..*/
		{
			data->simulation_stop = 1;
			death_time = philo->last_meal_time + data->time_to_die;
			pthread_mutex_unlock(&data->stop_mutex);
			/*Unlocks the stop mutex after modifying the flag.*/
			pthread_mutex_lock(&data->write_lock);
			/* Locks the write mutex to ensure exclusive access to the console for printing.*/
			printf("%lld %d died\n", death_time - data->start_time, philo->id);
			pthread_mutex_unlock(&data->write_lock);
			/*Unlocks the write mutex after printing.*/
			return (true);
		}
		pthread_mutex_unlock(&data->stop_mutex);
	}
	return (false);
/*
1. Sequence of events when a philosopher dies:
   a. Lock the stop mutex.
   b. Check if the simulation is already stopped.
   c. If not stopped, set the simulation stop flag.
   d. Calculate the exact death time.
   e. Unlock the stop mutex.
   f. Lock the write mutex.
   g. Print the death message.
   h. Unlock the write mutex.
   i. Return true to indicate death.

2. Order of operations:
   - The stop mutex is locked first to ensure that only one thread can declare a philosopher dead and stop the simulation.
   - The death time is calculated before unlocking the stop mutex to ensure accuracy.
   - The write mutex is locked separately and after the stop mutex to prevent deadlocks and 
   		allow other critical operations to proceed while waiting for console access.

3. Edge cases and race conditions:
   - The function checks `!data->simulation_stop` to ensure that only the first detected death is reported, preventing multiple death messages.
   - Mutex locks prevent race conditions where multiple threads might simultaneously detect deaths or interfere with the stopping process.

Key points and role in the simulation:
1. Accurate death detection: Ensures philosophers don't exceed their time limit without eating.
2. Thread safety: Uses mutexes to prevent race conditions in a multi-threaded environment.
3. Simulation control: Triggers the end of the simulation when a death is detected.
4. Informative output: Provides precise timing information about when a philosopher died.
5. Efficiency: Checks death condition without interfering with the main simulation loop.
*/
}

void	*death_clock_routine(void *arg)
/*This is a function designed to be run as a separate thread. 
It takes a void pointer as an argument and returns a void pointer, which is typical for pthread functions.*/
{
	t_data	*data;

	data = (t_data *)arg;
	/*The function casts the input argument to a t_data struct pointer. 
	This struct likely contains all the necessary information about the simulation state.*/
	if (data->num_philosophers == 1)
	{
		kill_single_philosopher(data);
		return (NULL);
	}
	while (1)
	/*This is an infinite loop that continuously checks two conditions:
		a. If any philosopher has died (check_philosopher_deaths)
		b. If all philosophers have eaten enough times (check_all_philosophers_ate_enough)
		If either of these conditions is true, the function returns, effectively ending the thread.*/
	{
		if (check_philosopher_deaths(data))
			return (NULL);
		if (check_all_philosophers_ate_enough(data))
			return (NULL);
		usleep(50);  // Check frequently
		/*After each check, the thread sleeps for 50 microseconds (usleep(50)). 
		This prevents the loop from consuming too much CPU time while still checking frequently.*/
	}
/*
Key points and observations:
1. This function is designed to run concurrently with the main simulation, constantly monitoring the state.
2. It handles the edge case of a single philosopher differently.
3. The function relies on other functions (`check_philosopher_deaths`, `check_all_philosophers_ate_enough`) to determine the simulation state.
4. It uses a short sleep time to balance responsiveness with CPU usage.

This "death clock" approach is a clever way to monitor the simulation state without complicating the main philosopher logic. 
It allows for quick detection of death or completion conditions without each philosopher needing to constantly check these global states.*/
}