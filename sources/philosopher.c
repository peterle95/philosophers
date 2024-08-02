/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:08:49 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/02 15:28:34 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	handle_single_philosopher(t_philosopher *philo, t_data *data)
{
	print_status(data, philo->id, "has taken a fork");
	accurate_sleep(data->time_to_die);
}

void	introduce_delay(t_philosopher *philo, t_data *data)
{
	if (philo->id % 2 == 0)
		usleep(data->time_to_eat * 500);
}

int	check_simulation_stop(t_data *data)
{
	int		stop;

	pthread_mutex_lock(&data->stop_mutex);
	stop = data->simulation_stop;
	pthread_mutex_unlock(&data->stop_mutex);
	return (stop);
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
	
	Purpose:
	philo->data is likely a pointer to the main t_data structure that contains all shared information about the simulation.
	By assigning it to a local variable data, we create a shortcut for easier access to this shared information throughout the function.
	This can make the code more readable and slightly more efficient, as we don't need to dereference philo every time we want to access the shared data.*/
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
	while (!check_simulation_stop(data))
	{
		think_and_take_forks(philo, data, left_fork, right_fork);
		eat(philo, data);
		release_forks_and_sleep(philo, data, left_fork, right_fork);
	}
	return (NULL);
}
