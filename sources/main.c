/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:55:31 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/13 19:02:25 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* int main(int ac, char **av)
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
} */

int    allocate_memory(t_program *program)
{
    printf("Allocating for %d philosophers\n", MAX_PHILOS);
    // Allocate memory for philosophers and forks
    program->philosophers = malloc(sizeof(t_philosopher) * MAX_PHILOS);
    if (!program->philosophers)
    {
        printf("Error: Memory allocation failed\n");
       // cleanup_program(&program);
        return 1;
    }
    program->forks = malloc(sizeof(pthread_mutex_t) * MAX_PHILOS);
if (program->forks == NULL) {
    fprintf(stderr, "Failed to allocate memory for forks\n");
    printf("Error: Memory allocation for forks failed\n");
    return 1;
}
program->philosopher_threads = malloc(sizeof(pthread_t) * MAX_PHILOS);
if (program->philosopher_threads == NULL) {
    printf("Error: Memory allocation for philosopher threads failed\n");
    free(program->forks);  // Free previously allocated memory
    return 1;
}
return(0);
}

int main(int ac, char **av)
{
    t_program program;  // Changed from t_program *program 
    
    if(allocate_memory(&program) != 0)
        return (EXIT_FAILURE);
    if(validate_input(ac, av) != 0)
        return(EXIT_FAILURE);
    // Initialize program structure with command line arguments
    if (init_program(&program, ac, av) != 0)
    {
        printf("Error: Invalid arguments\n");
        return 1;
    }

    // Start simulation
    if (start_simulation(&program) != 0)
    {
        printf("Error: Failed to start simulation\n");
      //  cleanup_program(&program);
        return 1;
    }

// Wait for monitor thread to finish


    // Wait for all philosopher threads to finish
    for (int i = 0; i < program.num_philosophers; i++)
    {
        if (pthread_join(program.philosopher_threads[i], NULL) != 0)
        {
            printf("Error: Failed to join philosopher thread\n");
        }
    }
    // Cleanup
    // cleanup_program(&program);
    free(program.forks);
    free(program.philosopher_threads);
    return 0;
}