/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_allocation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 20:30:39 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/19 14:45:52 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	allocate_memory(t_data *data)
{
	data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
	data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
	if (!data->philosophers || !data->forks)
	{
		printf("Error: Memory allocation failed\n");
		return (1);
	}
	return (0);
}

/*The program only needs to allocate space for philosophers and forks because these 
are the primary dynamic data structures required for the simulation. Let's break down why:

1. Philosophers:
   - Each philosopher is represented by a struct (t_philosopher) that contains information like their ID, meal count, last meal time, etc.
   - The number of philosophers is determined at runtime based on user input.
   - We need an array of these structures to keep track of all philosophers' states.

2. Forks:
   - Forks are represented by mutexes (pthread_mutex_t).
   - There is one fork for each philosopher.
   - We need an array of mutexes to represent all the forks.

These are the only dynamically sized data structures needed because:

1. The number of philosophers (and correspondingly, forks) is not known at compile time.
2. These structures need to persist throughout the simulation and be accessible by multiple threads.

Other elements of the program don't require dynamic allocation:

- Threads: While we create threads for each philosopher, the thread identifiers (pthread_t) are typically stack-allocated or part of the philosopher structure.
- Other mutexes (like write_lock and stop_mutex): These are single instances, not arrays, so they can be statically allocated as part of the main data structure.
- Timing information and counters: These are usually simple variables that don't need dynamic allocation.

By allocating only what's necessary, the program maintains efficiency in memory usage. It allocates just 
enough memory to represent the variable number of philosophers and their associated forks, while keeping 
other elements of the simulation in fixed-size structures or stack-allocated variables.


3. Death Clock: The death clock doesn't need separate memory allocation 
because it's typically implemented as a single thread that monitors all philosophers. 
It doesn't need an array or multiple instances.
It doesn't require dynamic memory allocation because it's just a single thread, and its 
ID can be stored in a stack-allocated variable within the function that creates it.*/


