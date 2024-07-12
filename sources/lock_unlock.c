/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:44:35 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/12 21:33:03 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

/* int lock_forks(t_philosopher *philo)
{
    // Always lock the lower-numbered fork first to prevent deadlock
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&philo->program->forks[philo->left_fork]);
        pthread_mutex_lock(&philo->program->forks[philo->right_fork]);
    }
    else
    {
        pthread_mutex_lock(&philo->program->forks[philo->right_fork]);
        pthread_mutex_lock(&philo->program->forks[philo->left_fork]);
    }

    // Check if the simulation has stopped while trying to lock forks
    if (philo->program->simulation_stop)
    {
        unlock_forks(philo);
        return 1;
    }

    return 0;
}

void unlock_forks(t_philosopher *philo)
{
    pthread_mutex_unlock(&philo->program->forks[philo->left_fork]);
    pthread_mutex_unlock(&philo->program->forks[philo->right_fork]);
} */

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    struct timeval current_time;

    while (!philo->program->simulation_stop)
    {
        debug_print("Philosopher %d is thinking\n", philo->id);
        usleep(1000); // Small delay to prevent busy waiting

        if (lock_forks(philo) == 0)
        {
            gettimeofday(&current_time, NULL);
            long long current_ms = current_time.tv_sec * 1000LL + current_time.tv_usec / 1000;
            
            debug_print("Philosopher %d started eating at %lld ms\n", philo->id, current_ms - philo->program->start_time);
            philo->last_meal_time = current_ms;
            philo->meals_eaten++;

            usleep(philo->program->time_to_eat * 1000); // Convert to microseconds

            unlock_forks(philo);
            debug_print("Philosopher %d finished eating\n", philo->id);

            if (philo->program->num_times_to_eat != -1 && philo->meals_eaten >= philo->program->num_times_to_eat)
            {
                debug_print("Philosopher %d has eaten enough times\n", philo->id);
                break;
            }

            debug_print("Philosopher %d is sleeping\n", philo->id);
            usleep(philo->program->time_to_sleep * 1000); // Convert to microseconds
        }
    }

    debug_print("Philosopher %d is exiting\n", philo->id);
    return NULL;
}

int lock_forks(t_philosopher *philo)
{
    debug_print("Philosopher %d trying to lock forks\n", philo->id);
    
    // Always lock the lower-numbered fork first to prevent deadlock
    if (philo->left_fork < philo->right_fork)
    {
        pthread_mutex_lock(&philo->program->forks[philo->left_fork]);
        debug_print("Philosopher %d locked left fork %d\n", philo->id, philo->left_fork);
        pthread_mutex_lock(&philo->program->forks[philo->right_fork]);
        debug_print("Philosopher %d locked right fork %d\n", philo->id, philo->right_fork);
    }
    else
    {
        pthread_mutex_lock(&philo->program->forks[philo->right_fork]);
        debug_print("Philosopher %d locked right fork %d\n", philo->id, philo->right_fork);
        pthread_mutex_lock(&philo->program->forks[philo->left_fork]);
        debug_print("Philosopher %d locked left fork %d\n", philo->id, philo->left_fork);
    }

    // Check if the simulation has stopped while trying to lock forks
    if (philo->program->simulation_stop)
    {
        debug_print("Philosopher %d unlocking forks due to simulation stop\n", philo->id);
        unlock_forks(philo);
        return 1;
    }

    debug_print("Philosopher %d successfully locked both forks\n", philo->id);
    return 0;
}

void unlock_forks(t_philosopher *philo)
{
    debug_print("Philosopher %d unlocking forks\n", philo->id);
    pthread_mutex_unlock(&philo->program->forks[philo->left_fork]);
    debug_print("Philosopher %d unlocked left fork %d\n", philo->id, philo->left_fork);
    pthread_mutex_unlock(&philo->program->forks[philo->right_fork]);
    debug_print("Philosopher %d unlocked right fork %d\n", philo->id, philo->right_fork);
}

int create_philosopher_threads(t_program *program)
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    program->start_time = start_time.tv_sec * 1000LL + start_time.tv_usec / 1000;

    for (int i = 0; i < program->num_philosophers; i++)
    {
        if (pthread_create(&program->philosopher_threads[i], NULL, philosopher_routine, &program->philosophers[i]) != 0)
        {
            printf("Error: Failed to create thread for philosopher %d\n", i + 1);
            return 1;
        }
    }
    return 0;
}