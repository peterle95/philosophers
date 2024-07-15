/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:54:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/15 14:24:36 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h> // printf is allowed
#include <string.h> // memset is allowed
#include <pthread.h>
#include <stdlib.h> // malloc & free
#include <sys/time.h> // gettimeofday
#include<unistd.h> // usleep & write
#include <limits.h>

#define MAX_PHILOS 1000

#define DEBUG 0  // Set to 0 to disable debug prints

#if DEBUG
#define debug_print(fmt, ...) \
    do { fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, \
                __LINE__, __func__, __VA_ARGS__); } while (0)
#else
#define debug_print(fmt, ...) do {} while (0)
#endif

typedef struct s_program
{
    int num_philosophers;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_times_to_eat;  // Optional, -1 if not specified
    
    pthread_t *philosopher_threads;
    pthread_mutex_t *forks_mutex;
    pthread_mutex_t write_mutex;  // For synchronized console output
    pthread_t *death_clock;
    
    struct s_philosopher *philosophers;
    
    long long start_time;  // Timestamp when simulation starts
    int simulation_stop;   // Flag to stop the simulation
} t_program;

typedef struct s_philosopher
{
    int id;
    int left_fork;
    int right_fork;
    int meals_eaten;
    long long last_meal_time;
    t_program *program;  // Pointer back to the main program struct
} t_philosopher;

// Initialization
int init_program(t_program *program, int ac, char **av);

// Validation
int validate_input(int ac, char **av);

// Utils
long long ft_atol(const char *str);

// Lock & Unlock Mutexes
int lock_forks(t_philosopher *philo);
void unlock_forks(t_philosopher *philo);

// Philosophers
int create_philosopher_threads(t_program *program);

// Death Clock
void *death_clock_routine(void *arg);


void *philosopher_routine(void *ac);
void cleanup_program(t_program *program);
int start_simulation(t_program *program);


#endif