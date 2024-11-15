/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:54:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/29 17:35:54 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <string.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>
# include <limits.h>
# include <stdbool.h>

typedef struct s_philosopher
{
	int				id;
	int				times_eaten;
	long			last_meal_time;
	pthread_t		thread_philo;
	struct s_data	*data;
}	t_philosopher;

typedef struct s_data
{
	int				num_philosophers;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	int				meals_to_eat;
	long			start_time;
	int				simulation_stop;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	write_lock;
	t_philosopher	*philosophers;
}	t_data;

// Utils
long	get_current_time(void);
void	print_status(t_data *data, int id, char *status);
long	ft_atol(const char *str);
void	set_simulation_stop(t_data *data);

// Cleanup
void	cleanup(t_data *data);

// Check
int		check_philosopher_deaths(t_data *data);
int		check_all_philosophers_ate_enough(t_data *data);

// Threads
int		create_and_run_threads(t_data *data);

// Init
int		init_data(t_data *data, int argc, char **argv);

// Philosopher
void	*philosopher_routine(void *arg);

// Death Clock
bool	is_philosopher_dead(t_philosopher *philo, t_data *data);
void	*death_clock_routine(void *arg);
bool	all_philosophers_ate_enough(t_data *data);

// Validate Input
int		validate_input(int argc, char **argv);

// Memory Allocation
int		allocate_memory(t_data *data);

// Eat Think And Sleep
void	accurate_sleep(long time_in_ms);
void	release_forks_and_sleep(t_philosopher *philo, t_data *data,
			int left_fork, int right_fork);
void	think_and_take_forks(t_philosopher *philo, t_data *data,
			int left_fork, int right_fork);
void	eat(t_philosopher *philo, t_data *data);

// Mutexes
int		initialize_stop_mutex(t_data *data);
int		initialize_write_lock(t_data *data);
void	destroy_fork_mutexes(t_data *data, int num_to_destroy);
int		initialize_fork_mutexes(t_data *data);

#endif
