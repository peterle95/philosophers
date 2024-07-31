/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:03:05 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/31 15:16:33 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	ft_atoll(const char *str)
{
	long	result;
	int		sign;
	int		i;

	result = 0;
	sign = 1;
	i = 0;
	// Handle negative numbers
	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
    // Convert string to long long
	while (str[i] != '\0')
	{
		/* if (str[i] < '0' || str[i] > '9')
			return (0);
		--> this should be checked in invalid number already */
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}


/*Great question. We add tv_sec and tv_usec together to get a more precise time measurement. Let's break it down:

1. tv_sec represents the number of whole seconds since the Unix epoch.
2. tv_usec represents the additional microseconds within the current second.

We add these together (after appropriate conversion) to get a single, high-precision time value in milliseconds.

Here's why this is necessary:

1. Precision: Many operations in the Dining Philosophers problem require millisecond-level precision. Using only seconds (tv_sec) would be too coarse.

2. Continuity: By combining seconds and microseconds, we get a continuous time value, avoiding "jumps" that might occur if we only used seconds.

Let's look at an example:

Suppose gettimeofday returns these values:
tv_sec = 1627984800 (August 3, 2021, 12:00:00 PM UTC)
tv_usec = 500000 (0.5 seconds)

The function would calculate:
(1627984800 * 1000LL) + (500000 / 1000) = 1627984800000 + 500 = 1627984800500

This result, 1627984800500, represents August 3, 2021, 12:00:00.500 PM UTC in milliseconds since the Unix epoch.

If we had only used tv_sec, we would get 1627984800000, losing the half-second precision.

In the context of the Dining Philosophers:
- If a philosopher's "time to die" is 800 ms, and their last meal was at 1627984800000.
- The current time is 1627984800500.
- We can accurately determine that 500 ms have passed, not a full second.

This level of precision is crucial for accurately simulating the timing of philosophers' actions and detecting when a philosopher has "died" due to starvation.*/
long	get_current_time(void)
{
	struct timeval	tv;
	/*Declares a variable tv of type struct timeval. This structure contains two members: tv_sec (seconds) and tv_usec (microseconds).*/
	if (gettimeofday(&tv, NULL) == -1)
	/*Calls the gettimeofday function, which fills the tv structure with the current time.
	The first argument is a pointer to tv.
	The second argument is NULL (often used for timezone, but it's typically NULL in modern systems).*/
	{
		printf("Error: gettimeofday failed\n");
		return (-1);
	}
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
	/*It converts the time to milliseconds:

	tv.tv_sec * 1000LL: Converts seconds to milliseconds (1000LL ensures long long multiplication).
	tv.tv_usec / 1000: Converts microseconds to milliseconds.
	The sum of these gives the total milliseconds since the Unix epoch (January 1, 1970).*/
}

void	print_status(t_data *data, int id, char *status)
{
	long long	current_time;

	pthread_mutex_lock(&data->stop_mutex);
	if (!data->simulation_stop)
	{
		current_time = get_current_time();
		pthread_mutex_lock(&data->write_lock);
		printf("%lld %d %s\n", current_time - data->start_time, id, status);
		pthread_mutex_unlock(&data->write_lock);
	}
	pthread_mutex_unlock(&data->stop_mutex);
}

void	set_simulation_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}
