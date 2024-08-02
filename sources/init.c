/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:07:27 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/01 14:20:57 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	parse_arguments(t_data *data, int argc, char **argv)
{
	data->num_philosophers = atoi(argv[1]);
	data->time_to_die = ft_atoll(argv[2]);
	data->time_to_eat = ft_atoll(argv[3]);
	data->time_to_sleep = ft_atoll(argv[4]);
	if (argc == 6)
		data->meals_to_eat = atoi(argv[5]);
	if (data->num_philosophers < 1 || data->time_to_die
		< 0 || data->time_to_eat < 0 || data->time_to_sleep
		< 0 || (argc == 6 && data->meals_to_eat < 0))
	{
		printf("Error: Invalid argument values\n");
		return (1);
	}
	return (0);
}

int	initialize_mutexes(t_data *data)
{
	if (initialize_fork_mutexes(data) != 0)
		return (1);
	if (initialize_write_lock(data) != 0)
		return (1);
	if (initialize_stop_mutex(data) != 0)
		return(1);
	return (0);
}

void	initialize_philosophers(t_data *data)
{
	int		i;

	i = 0;
	data->start_time = get_current_time();
	/*This line sets the start time of the simulation to the current time.
	The start time is accessed here to establish a common reference point for all philosophers. 
	This will be used later to calculate relative times for events in the simulation.*/
	while (i < data->num_philosophers)
	{
		/*So when accessing fields, when do we use dot and when do we use arrow?
			• If you have a struct, use dot (.).
			• If you have a pointer to a struct, use arrow (->).
				The format data->philosophers[i].[...] is used because:

					1. data is a pointer to the main structure containing all simulation data.
					2. philosophers is an array within that structure, holding information for each philosopher.
					3. [i] accesses the i-th philosopher in this array.
					4. The dot notation . then accesses individual properties of that philosopher.

		You could also write:
			(*data).philosophers[i].id = i + 1;
			(*data).philosophers[i].times_eaten = 0;
			(*data).philosophers[i].last_meal_time = (*data).start_time;
			(*data).philosophers[i].data = data;
		*/
		data->philosophers[i].id = i + 1; // Sets a unique ID for each philosopher (starting from 1).
		data->philosophers[i].times_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		/*Sets the last meal time to the start time of the simulation. This is important because it assumes all philosophers 
		start in a "just eaten" state, giving them the full time_to_die duration before they need to eat again.*/
		data->philosophers[i].data = data;
		/*Gives each philosopher a reference back to the main data structure, which contains global information about the simulation.
		
		This line crucial for the structure and functionality of the program. Let's break it down in more detail:

		1. Purpose:
		   This line creates a circular reference. It allows each philosopher to have access to the main program data structure.

		2. Necessity:
		   - Each philosopher runs as a separate thread, executing the `philosopher_routine` function.
		   - Within this routine, philosophers need access to global information such as the time to eat, time to sleep, mutexes for forks, and other shared resources.
		   - Instead of passing multiple parameters to each philosopher thread, this single pointer provides access to all necessary information.

		3. Structure:
		   - `data` is a pointer to the main `t_data` structure, which contains all program information.
		   - `philosophers` is an array within this structure, where each element represents a philosopher.
		   - Each philosopher (represented by `t_philosopher`) has its own `data` member, which points back to the main `t_data` structure.

		4. Usage:
		   In the `philosopher_routine` function, you'll likely see code like:
		   ```c
		   t_philosopher *philo = (t_philosopher *)arg;
		   t_data *data = philo->data;
		   ```
		   This allows the philosopher to access shared resources and settings, like:
		   ```c
		   data->time_to_eat
		   data->forks[fork_index]
		   data->write_lock
		   ```

		5. Benefits:
		   - Encapsulation: Each philosopher has everything it needs through a single pointer.
		   - Flexibility: If you need to add more shared data, you only need to modify the main structure.
		   - Consistency: Ensures all philosophers are working with the same set of shared data.

		6. Potential pitfalls:
		   - This creates a circular reference, which could complicate memory management in some scenarios 
		   (though not typically an issue in this kind of program).
		   - It's crucial to use proper synchronization (mutexes) when accessing shared data to avoid race conditions.

		In summary, `data->philosophers[i].data = data;` is a design choice that simplifies data access for each philosopher 
		thread, allowing them to operate semi-independently while still having access to all necessary shared information. 
		This approach is particularly useful in multithreaded programs like the dining philosophers problem, where multiple 
		entities need controlled access to shared resources.*/
		i++;
	}
	data->simulation_stop = 0;
	// This line initializes the flag that will be used to stop the simulation when set to 1.
}

int	init_data(t_data *data, int argc, char **argv)
{
	if (parse_arguments(data, argc, argv) != 0)
		return (1);
	if (allocate_memory(data) != 0)
		return (1);
	if (initialize_mutexes(data) != 0)
	{
		free(data->forks);
		free(data->philosophers);
		return (1);
	}
	initialize_philosophers(data);
	return (0);
}
