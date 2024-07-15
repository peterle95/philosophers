/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:58:22 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/15 14:31:11 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int assign_values(t_program *program, int ac, char **av)
{
    program->num_philosophers = ft_atol(av[1]);
    program->time_to_die = ft_atol(av[2]);
    program->time_to_eat = ft_atol(av[3]);
    program->time_to_sleep = ft_atol(av[4]);
    if(ac == 6)
        program->num_times_to_eat = ft_atol(av[5]);
    else
        program->num_times_to_eat = -1;
    program->simulation_stop = 0;
    return(0);
}

int init_threads(t_program *program)
{
    for (int i = 0; i < program->num_philosophers; i++)
    {
        if (pthread_mutex_init(&program->forks_mutex[i], NULL) != 0)
        {
            printf("Error: Mutex initialization failed\n");
           // cleanup_program(program);
            return 1;
        }
    }
    if (pthread_mutex_init(&program->write_mutex, NULL) != 0)
    {
        printf("Error: Write mutex initialization failed\n");
        // cleanup_program(program);
        return 1;
    }
    return(0);
}

int init_philosophers(t_program *program)
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    long long current_ms = current_time.tv_sec * 1000LL + current_time.tv_usec / 1000;

    for (int i = 0; i < program->num_philosophers; i++)
    {
        program->philosophers[i].id = i + 1;
        program->philosophers[i].left_fork = i;
        program->philosophers[i].right_fork = (i + 1) % program->num_philosophers;
        program->philosophers[i].meals_eaten = 0;
        program->philosophers[i].last_meal_time = current_ms;
        program->philosophers[i].program = program;
    }
    return(0);
}

/* int    init_program(t_program *program, int ac, char **av)
{
    if (assign_values(program, ac, av) != 0)
        return(EXIT_FAILURE);
    if(init_threads(program) != 0)
        return(EXIT_FAILURE);
    if(init_philosophers(program) != 0)
        return(EXIT_FAILURE);
    return(0);
} */

int init_program(t_program *program, int ac, char **av)
{
    if (assign_values(program, ac, av) != 0)
        return 1;
    if(init_threads(program) != 0)
    {
        free(program->forks_mutex);
        free(program->philosopher_threads);
        return 1;
    }
    if(init_philosophers(program) != 0)
        return(EXIT_FAILURE);
    return 0;
}