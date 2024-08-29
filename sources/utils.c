/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:03:05 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/29 17:32:20 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Converts a string to a long integer
long	ft_atol(const char *str)
{
	int		sign;
	int		i;
	long	result;

	result = 0;
	sign = 1;
	i = 0;
	if (str[0] == '-')
	{
		sign = -1;
		i++;
	}
	while (str[i] != '\0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

// Gets the current time in milliseconds since the Unix epoch
long	get_current_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		return (-1);
	}
	return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}

// Prints the status of a philosopher with a timestamp
void	print_status(t_data *data, int id, char *status)
{
	long	current_time;

	pthread_mutex_lock(&data->stop_mutex);
	if (!data->simulation_stop)
	{
		current_time = get_current_time();
		pthread_mutex_lock(&data->write_lock);
		printf("%ld %d %s\n", current_time - data->start_time, id, status);
		pthread_mutex_unlock(&data->write_lock);
	}
	pthread_mutex_unlock(&data->stop_mutex);
}

// Sets the simulation stop flag to indicate the end of the simulation
void	set_simulation_stop(t_data *data)
{
	pthread_mutex_lock(&data->stop_mutex);
	data->simulation_stop = 1;
	pthread_mutex_unlock(&data->stop_mutex);
}
