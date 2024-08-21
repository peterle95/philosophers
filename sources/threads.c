/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:47:52 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/19 14:01:19 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	create_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_create(&data->philosophers[i].thread_philo, NULL,
				philosopher_routine, &data->philosophers[i]) != 0)
				/*This is the core of the function, using pthread_create to spawn a new thread for each philosopher.
				Let's break down the arguments:
				a. &data->philosophers[i].thread_philo: Where to store the new thread ID.
				b. NULL: Default thread attributes.
				c. philosopher_routine: The function each thread will run.
				d. &data->philosophers[i]: Pointer to the specific philosopher's data, passed to the routine.
											This is a pointer to the t_philosopher struct for the i-th philosopher.
				The cast in the philosopher_routine function is still necessary. Here is why:
					pthread_create expects a function of type void *(*start_routine) (void *).
					When the thread starts, it calls this function with a void * argument.
					Inside philosopher_routine, you need to cast this void * back to t_philosopher
					
					
		╰(*°▽°*)╯EXPLANATION OF LIBRARY FUNCTION╰(*°▽°*)╯
		Let's break down this function declaration for `pthread_create` and explain its purpose and components.

		High-level overview:
		This is the declaration of the `pthread_create` function, which is used to create a new thread in a multi-threaded program. 
		It's a fundamental function in POSIX thread (pthread) programming.

		Now, let's dive into the details:

--->	extern int pthread_create (pthread_t *__restrict __newthread, <---
			   const pthread_attr_t *__restrict __attr,
			   void *(*__start_routine) (void *),
			   void *__restrict __arg) __THROWNL __nonnull ((1, 3));

		1. Function Signature:
		   extern int pthread_create(...)
		   - `extern` indicates that this function is defined externally, likely in a system library.
		   - It returns an integer, typically 0 for success and an error code for failure.

		2. Parameters:
		   a. `pthread_t *__restrict __newthread`
		      - This is where the ID of the newly created thread will be stored.
		      - `__restrict` is a compiler optimization hint suggesting this pointer doesn't alias.

		   b. `const pthread_attr_t *__restrict __attr`
		      - Pointer to a structure defining thread attributes.
		      - If NULL, default attributes are used.

		   c. `void *(*__start_routine) (void *)`
		      - This is a pointer to the function that the new thread will execute.
		      - It takes a void pointer as an argument and returns a void pointer.

		   d. `void *__restrict __arg`
		      - This is the argument that will be passed to the start routine.
		      - It's a void pointer, allowing any type of data to be passed.

		3. Function Attributes:
		   - `__THROWNL`: Likely a macro indicating this function doesn't throw exceptions.
		   - `__nonnull ((1, 3))`: Indicates that the 1st and 3rd arguments (newthread and start_routine) must not be NULL.

		Clever aspects:
		1. The use of `void *` for both the function argument and return type allows for great flexibility in what can be passed to and returned from the thread function.
		2. The `__restrict` keyword is used to help the compiler optimize the code, assuming these pointers are not aliased.

		Analogies:
		Imagine `pthread_create` as a "birth certificate" office for threads:
		- `__newthread` is like the birth certificate itself, where the new thread's identity is recorded.
		- `__attr` is like optional instructions for raising the thread (if NULL, default parenting guidelines are used).
		- `__start_routine` is like assigning the thread its life's mission.
		- `__arg` is like giving the thread some starting resources or information for its mission.

		In summary, `pthread_create` is a powerful function that spawns a new thread, specifying where to 
		store its ID, what attributes it should have, what function it should run, and what initial data it should have. 
		It's the foundation of creating concurrent execution paths in a program.
		╰(*°▽°*)╯END OF OF LIBRARY FUNCTION╰(*°▽°*)╯*/
		{
			printf("Error creating thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	create_death_thread(t_data *data, pthread_t *death_clock_thread)
// The death clock thread monitors the philosophers to check if any of them have "died" (i.e., haven't eaten for too long).
/*This function takes two parameters:
a. t_data *data: A pointer to the main data structure of the simulation.
b. pthread_t *death_clock_thread: A pointer to store the ID of the new thread.*/
{
	if (pthread_create(death_clock_thread, NULL, death_clock_routine, data) != 0)
	/*This is the core of the function, using pthread_create to spawn the death clock thread. Let's break down the arguments:
		a. death_clock_thread: Where to store the new thread ID.
		b. NULL: Default thread attributes.
		c. death_clock_routine: The function this thread will run.
		d. data: The main data structure, passed to the routine.
	
	How pthread_create works in depth:

	It allocates a new thread in the calling process.
	It initializes the thread's attributes according to the attr argument (NULL here, so default attributes).
	It creates a new execution context for the thread.
	It makes the new thread execute the death_clock_routine function, passing data as its argument.
		Passing the entire data structure allows the death clock routine to access all necessary information
	The new thread is created in a joinable state unless specified otherwise in attributes.
	If successful, it stores the ID of the new thread in the location pointed to by death_clock_thread.
*/
	{
		printf("Error creating death clock thread\n");
		return (1);
	}
	return (0);
}

int	join_philosopher_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_join(data->philosophers[i].thread_philo, NULL) != 0)
		{
			printf("Error joining thread\n");
			return (1);
		}
		i++;
	}
	return (0);
}

int	create_and_run_threads(t_data *data)
{
	pthread_t	death_clock_thread;

	if (create_philosopher_threads(data) != 0)
		return (1);
	usleep(1000);
	/*It introduces a small delay to allow all philosopher threads to be created and initialized before the simulation officially starts.
	It helps to reduce the chance of race conditions that might occur if the start time is set immediately after thread creation.
	It can help distribute the initial actions of the philosophers, preventing 
		them from all starting simultaneously, which could lead to immediate deadlocks in some implementations.*/
	data->start_time = get_current_time();
	// why is there here and in the "void	initialize_philosophers(t_data *data)", start_time fetching?
	if (create_death_thread(data, &death_clock_thread) != 0)
		return (1);
	if (join_philosopher_threads(data) != 0)
		return (1);
	if (pthread_join(death_clock_thread, NULL) != 0)
	{
		printf("Error joining death clock thread\n");
		return (1);
	}
	return (0);
}