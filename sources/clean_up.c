/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean_up.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 19:23:39 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/15 14:34:17 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void cleanup_program(t_program *program)
{
    int i;

    // Destroy mutexes
    for (i = 0; i < program->num_philosophers; i++)
    {
        pthread_mutex_destroy(&program->forks_mutex[i]);
    }
    pthread_mutex_destroy(&program->write_mutex);

    // Free allocated memory
    if (program->philosophers)
        free(program->philosophers);
    if (program->forks_mutex)
        free(program->forks_mutex);
    if (program->philosopher_threads)
        free(program->philosopher_threads);

    // Set pointers to NULL after freeing
    program->philosophers = NULL;
    program->forks_mutex = NULL;
    program->philosopher_threads = NULL;
}