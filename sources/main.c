/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:55:31 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/11 16:00:40 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int ac, char **av)
{
    t_program *program;

    if(validate_input(ac, av) != 0)
        return(EXIT_FAILURE);
    // Initialize program structure with command line arguments
    if (init_program(&program, ac, av) != 0)
    {
        printf("Error: Invalid arguments\n");
        return 1;
    }

    // Allocate memory for philosophers and forks
    program->philosophers = malloc(sizeof(t_philosopher) * program->num_philosophers);
    program->philosopher_threads = malloc(sizeof(pthread_t) * program->num_philosophers);
    program->forks = malloc(sizeof(pthread_mutex_t) * program->num_philosophers);

    if (!program->philosophers || !program->philosopher_threads || !program->forks)
    {
        printf("Error: Memory allocation failed\n");
        cleanup_program(&program);
        return 1;
    }

    // Start simulation
    if (start_simulation(&program) != 0)
    {
        printf("Error: Failed to start simulation\n");
        cleanup_program(&program);
        return 1;
    }

    // Wait for all threads to finish
    for (int i = 0; i < program->num_philosophers; i++)
    {
        if (pthread_join(program->philosopher_threads[i], NULL) != 0)
        {
            printf("Error: Failed to join thread\n");
        }
    }

    // Cleanup
    cleanup_program(&program);

    return 0;
}