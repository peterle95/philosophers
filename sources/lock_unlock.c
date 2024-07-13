/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:44:35 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/13 19:08:01 by pmolzer          ###   ########.fr       */
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

void *death_clock_routine(void *arg)
{
    t_program *program = (t_program *)arg;
    struct timeval current_time;
    long long current_ms;

    while (!program->simulation_stop)
    {
        for (int i = 0; i < program->num_philosophers; i++)
        {
            gettimeofday(&current_time, NULL);
            current_ms = current_time.tv_sec * 1000LL + current_time.tv_usec / 1000;

            if (current_ms - program->philosophers[i].last_meal_time > program->time_to_die)
            {
                pthread_mutex_lock(&program->write_mutex);
                printf("%lld %d died\n", current_ms - program->start_time, program->philosophers[i].id);
                program->simulation_stop = 1;
                pthread_mutex_unlock(&program->write_mutex);
                return NULL;
            }
        }
        usleep(1000); // Sleep for 1ms to avoid busy waiting
    }
    return NULL;
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    struct timeval current_time;

    while (!philo->program->simulation_stop)
    {
        if (philo->program->simulation_stop) break;
        
        debug_print("Philosopher %d is thinking\n", philo->id);
        
        if (lock_forks(philo) == 0)
        {
            if (philo->program->simulation_stop) 
            {
                unlock_forks(philo);
                break;
            }
            
            gettimeofday(&current_time, NULL);
            long long current_ms = current_time.tv_sec * 1000LL + current_time.tv_usec / 1000;
            
            pthread_mutex_lock(&philo->program->write_mutex);
            if (!philo->program->simulation_stop)
            {
                printf("%lld %d has taken a fork\n", current_ms - philo->program->start_time, philo->id);
                printf("%lld %d is eating\n", current_ms - philo->program->start_time, philo->id);
                philo->last_meal_time = current_ms;
            }
            pthread_mutex_unlock(&philo->program->write_mutex);

            usleep(philo->program->time_to_eat * 1000);
            unlock_forks(philo);

            if (philo->program->simulation_stop) break;

            if (philo->program->num_times_to_eat != -1 && ++philo->meals_eaten >= philo->program->num_times_to_eat)
            {
                break;
            }

            pthread_mutex_lock(&philo->program->write_mutex);
            if (!philo->program->simulation_stop)
            {
                printf("%lld %d is sleeping\n", current_ms - philo->program->start_time, philo->id);
            }
            pthread_mutex_unlock(&philo->program->write_mutex);

            usleep(philo->program->time_to_sleep * 1000);
        }
    }

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

int start_simulation(t_program *program)
{
    pthread_t death_clock_thread;

    if (create_philosopher_threads(program) != 0)
    {
        printf("Error: Failed to create philosopher threads\n");
       // cleanup_program(&program);
        return 1;
    }

    if (pthread_create(&death_clock_thread, NULL, death_clock_routine, &program) != 0)
    {
        printf("Error: Failed to create monitor thread\n");
        return 1;
    }
    if (pthread_join(death_clock_thread, NULL) != 0)
    {
        printf("Error: Failed to join monitor thread\n");
    }
    return(0);
}