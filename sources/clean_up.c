/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:23:39 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/13 19:31:07 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void cleanup_program(t_program *program)
{
    int i;

    // Destroy mutexes
    for (i = 0; i < program->num_philosophers; i++)
    {
        pthread_mutex_destroy(&program->forks[i]);
    }
    pthread_mutex_destroy(&program->write_mutex);

    // Free allocated memory
    if (program->philosophers)
        free(program->philosophers);
    if (program->forks)
        free(program->forks);
    if (program->philosopher_threads)
        free(program->philosopher_threads);

    // Set pointers to NULL after freeing
    program->philosophers = NULL;
    program->forks = NULL;
    program->philosopher_threads = NULL;
}

/* void    join_threads(t_program *program)
{
    pthread_t death_clock_thread;
    
    for (int i = 0; i < program->num_philosophers; i++)
    {
        if (pthread_join(program->philosopher_threads[i], NULL) != 0)
        {
            printf("Error: Failed to join philosopher thread\n");
        }
    }
    if (pthread_join(death_clock_thread, NULL) != 0)
    {
        printf("Error: Failed to join monitor thread\n");
    }
} */