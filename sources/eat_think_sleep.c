/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_think_sleep.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 23:44:56 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/04 06:08:51 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/*Precision: Standard sleep functions (like sleep or usleep) aren't always precise, especially for short durations. 
This function aims to provide more accurate timing.

Adaptive sleeping: By using shorter sleep intervals as it gets closer to the target time, it can more accurately hit the desired wake-up time.

Efficiency: It balances between accuracy and CPU usage. Longer sleeps (500μs) are used when possible to reduce CPU wake-ups, 
while shorter sleeps (100μs) are used for fine-tuning at the end.

Avoiding oversleep: By checking the time frequently and using short sleep intervals, it minimizes the chance of sleeping longer than intended.

Handling system interruptions: If the sleep is interrupted (e.g., by a system signal), this 
method will continue trying to sleep until the full duration has passed.

Simulation accuracy: In the context of the dining philosophers problem, accurate timing is crucial for 
simulating real-time constraints and ensuring fair, consistent behavior across multiple runs of the simulation.

This implementation shows attention to detail in creating a more precise timing mechanism, which is particularly important in 
multi-threaded applications where small timing discrepancies can lead to different outcomes or race conditions.*/
void	accurate_sleep(long long time_in_ms)
{
	long long	start;
	long long	end;
	/*start: Stores the starting time of the sleep.
	end: Stores the target end time of the sleep.*/

	start = get_current_time();
	end = start + time_in_ms;
	/*Sets start to the current time.
	Calculates end by adding the desired sleep duration to start.*/
	while (get_current_time() < end)
	/*Continues until the current time reaches or exceeds the target end time.*/
	{
		if (end - get_current_time() > 1)
			usleep(500);
		else
			usleep(100);
	/*If more than 1ms remains: sleep for 500 microseconds (0.5ms).
	If 1ms or less remains: sleep for 100 microseconds (0.1ms).*/
	}
}

void	think_and_take_forks(t_philosopher *philo, t_data *data,
				int left_fork, int right_fork)
{
	print_status(data, philo->id, "is thinking");
	 if (philo->id % 2 == 0) {
        // Even philosophers take right fork first
        pthread_mutex_lock(&data->forks[right_fork]);
		print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[left_fork]);
		print_status(data, philo->id, "has taken a fork");
    } else {
        // Odd philosophers take left fork first
        pthread_mutex_lock(&data->forks[left_fork]);
		print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[right_fork]);
		print_status(data, philo->id, "has taken a fork");
    }
}

void	eat(t_philosopher *philo, t_data *data)
{
	philo->last_meal_time = get_current_time();
	print_status(data, philo->id, "is eating");
	accurate_sleep(data->time_to_eat);
	philo->times_eaten++;
}

void	release_forks_and_sleep(t_philosopher *philo, t_data *data,
				int left_fork, int right_fork)
{
	pthread_mutex_unlock(&data->forks[left_fork]);
	pthread_mutex_unlock(&data->forks[right_fork]);
	print_status(data, philo->id, "is sleeping");
	accurate_sleep(data->time_to_sleep);
}
