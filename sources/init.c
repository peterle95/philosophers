/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:07:27 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/01 10:58:42 by pmolzer          ###   ########.fr       */
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
	while (i < data->num_philosophers)
	{
		/*So when accessing fields, when do we use dot and when do we use arrow?
			• If you have a struct, use dot (.).
			• If you have a pointer to a struct, use arrow (->).
		You could also write:
			(*data).philosophers[i].id = i + 1;
			(*data).philosophers[i].times_eaten = 0;
			(*data).philosophers[i].last_meal_time = (*data).start_time;
			(*data).philosophers[i].data = data;
		*/
		data->philosophers[i].id = i + 1;
		data->philosophers[i].times_eaten = 0;
		data->philosophers[i].last_meal_time = data->start_time;
		data->philosophers[i].data = data;
		i++;
	}
	data->simulation_stop = 0;
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
