/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 14:58:22 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/11 22:40:26 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int init_threads(t_program *program)
{
    for (int i = 0; i < program->num_philosophers; i++)
    {
        if (pthread_mutex_init(&program->forks[i], NULL) != 0)
        {
            printf("Error: Mutex initialization failed\n");
            cleanup_program(&program);
            return 1;
        }
    }
    if (pthread_mutex_init(&program->write_mutex, NULL) != 0)
    {
        printf("Error: Write mutex initialization failed\n");
        cleanup_program(&program);
        return 1;
    }
    return(0);
}

int init_philosphers(t_program *program)
{
    for (int i = 0; i < program->num_philosophers; i++)
    {
        program->philosophers[i].id = i + 1;
        program->philosophers[i].left_fork = i;
        program->philosophers[i].right_fork = (i + 1) % program->num_philosophers;
        program->philosophers[i].meals_eaten = 0;
        program->philosophers[i].last_meal_time = 0;
        program->philosophers[i].program = &program;
    }
    return(0);
}

void    init_program(t_program *program, int ac, char **av)
{
    if(init_threads(program) != 0)
        return(EXIT_FAILURE);
    if(init_philosophers(program) != 0)
        return(EXIT_FAILURE);
}