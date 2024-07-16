/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:54:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:29:45 by pmolzer          ###   ########.fr       */
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
#include <stdbool.h>

typedef struct s_philosopher {
    int id;
    int times_eaten;
    long long last_meal_time;
    pthread_t thread;
    struct s_data *data;
} t_philosopher;

typedef struct s_data {
    int num_philosophers;
    long long time_to_die;
    long long time_to_eat;
    long long time_to_sleep;
    int meals_to_eat;
    long long start_time;
    int simulation_stop;
    pthread_mutex_t stop_mutex;
    pthread_mutex_t *forks;
    pthread_mutex_t write_lock;
    
    t_philosopher *philosophers;
} t_data;

// Utils
long long get_current_time(void);
void accurate_sleep(long long time_in_ms);
void print_status(t_data *data, int id, char *status);
long long ft_atoll(const char *str);

// Cleanup
void cleanup(t_data *data);

// Init
int init_data(t_data *data, int argc, char **argv);

// Philosopher
void *philosopher_routine(void *arg);

// Death Clock
bool death_clock(t_philosopher *philo, t_data *data);
void *monitor_routine(void *arg);

// Validate Input
int validate_input(int argc, char **argv);

#endif