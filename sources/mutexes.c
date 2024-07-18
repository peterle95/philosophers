#include "../includes/philo.h"

int	initialize_fork_mutexes(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->num_philosophers)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Mutex initialization failed\n");
			destroy_fork_mutexes(data, i);
			return (1);
		}
		i++;
	}
	return (0);
}

void	destroy_fork_mutexes(t_data *data, int num_to_destroy)
{
	int		i;

	i = 0;
	while (i < num_to_destroy)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
}

int	initialize_write_lock(t_data *data)
{
	if (pthread_mutex_init(&data->write_lock, NULL) != 0)
	{
		printf("Error: Write lock mutex initialization failed\n");
		destroy_fork_mutexes(data, data->num_philosophers);
		return (1);
	}
	return (0);
}

int	initialize_stop_mutex(t_data *data)
{
	return (pthread_mutex_init(&data->stop_mutex, NULL));
}
