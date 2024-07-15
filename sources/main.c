/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:36:15 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/15 17:12:31 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:54:04 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/15 15:46:04 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

long long get_current_time(void)
{
    struct timeval tv;
    
    if (gettimeofday(&tv, NULL) == -1)
    {
        printf("Error: gettimeofday failed\n");
        return -1;
    }
    return (tv.tv_sec * 1000LL + tv.tv_usec / 1000);
}
void accurate_sleep(long long time_in_ms)
{
    long long start = get_current_time();
    long long end = start + time_in_ms;
    while (get_current_time() < end)
    {
        if (end - get_current_time() > 1)
            usleep(500);
        else
            usleep(100);
    }
}

bool all_philosophers_ate_enough(t_data *data)
{
    if (data->meals_to_eat == -1)
        return false;

    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (data->philosophers[i].times_eaten < data->meals_to_eat)
            return false;
    }
    return true;
}

void cleanup(t_data *data)
{
    int i;

    if (data->philosophers)
    {
        free(data->philosophers);
        data->philosophers = NULL;
    }

    if (data->forks)
    {
        for (i = 0; i < data->num_philosophers; i++)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
        data->forks = NULL;
    }

    pthread_mutex_destroy(&data->write_lock);
}

void print_status(t_data *data, int id, char *status)
{
    pthread_mutex_lock(&data->stop_mutex);
    if (!data->simulation_stop)
    {
        long long current_time = get_current_time();
        pthread_mutex_lock(&data->write_lock);
        printf("%lld %d %s\n", current_time - data->start_time, id, status);
        pthread_mutex_unlock(&data->write_lock);
    }
    pthread_mutex_unlock(&data->stop_mutex);
}

bool check_philosopher_death(t_philosopher *philo, t_data *data)
{
    long long current_time = get_current_time();
    long long time_since_last_meal = current_time - philo->last_meal_time;

    if (time_since_last_meal >= data->time_to_die)
    {
        pthread_mutex_lock(&data->stop_mutex);
        if (!data->simulation_stop)
        {
            data->simulation_stop = 1;
            pthread_mutex_unlock(&data->stop_mutex);
            pthread_mutex_lock(&data->write_lock);
            printf("%lld %d died\n", data->time_to_die + philo->last_meal_time - data->start_time, philo->id);
            pthread_mutex_unlock(&data->write_lock);
            return true;
        }
        pthread_mutex_unlock(&data->stop_mutex);
    }
    return false;
}

void *monitor_routine(void *arg)
{
    t_data *data = (t_data *)arg;
    
    if (data->num_philosophers == 1)
    {
        accurate_sleep(data->time_to_die);
        printf("%lld 1 died\n", data->time_to_die);
        data->simulation_stop = 1;
        return NULL;
    }

    while (1)
    {
        for (int i = 0; i < data->num_philosophers; i++)
        {
            if (check_philosopher_death(&data->philosophers[i], data))
                return NULL;
        }
        if (all_philosophers_ate_enough(data))
        {
            pthread_mutex_lock(&data->stop_mutex);
            data->simulation_stop = 1;
            pthread_mutex_unlock(&data->stop_mutex);
            printf("All philosophers have eaten enough times\n");
            return NULL;
        }
        usleep(50);  // Check even more frequently
    }
}

void *philosopher_routine(void *arg)
{
    t_philosopher *philo = (t_philosopher *)arg;
    t_data *data = philo->data;
    int left_fork = philo->id - 1;
    int right_fork = philo->id % data->num_philosophers;

    if (data->num_philosophers == 1)
    {
        print_status(data, philo->id, "has taken a fork");
        accurate_sleep(data->time_to_die);
        return NULL;
    }

    if (philo->id % 2 == 0)
        usleep(1000); // Delay even-numbered philosophers slightly

    while (1)
    {
        pthread_mutex_lock(&data->stop_mutex);
        if (data->simulation_stop)
        {
            pthread_mutex_unlock(&data->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&data->stop_mutex);

        // Think
        print_status(data, philo->id, "is thinking");
        
        // Pick up forks
        pthread_mutex_lock(&data->forks[left_fork]);
        print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&data->forks[right_fork]);
        print_status(data, philo->id, "has taken a fork");
        
        // Eat
        philo->last_meal_time = get_current_time();
        print_status(data, philo->id, "is eating");
        accurate_sleep(data->time_to_eat);
        philo->times_eaten++;

        // Put down forks
        pthread_mutex_unlock(&data->forks[left_fork]);
        pthread_mutex_unlock(&data->forks[right_fork]);
        
        // Sleep
        print_status(data, philo->id, "is sleeping");
        accurate_sleep(data->time_to_sleep);
    }

    return NULL;
}

int init_data(t_data *data, int argc, char **argv)
{
    if (argc < 5 || argc > 6)
    {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    data->num_philosophers = atoi(argv[1]);
    data->time_to_die = atoll(argv[2]);
    data->time_to_eat = atoll(argv[3]);
    data->time_to_sleep = atoll(argv[4]);
    data->meals_to_eat = (argc == 6) ? atoi(argv[5]) : -1;

    if (data->num_philosophers < 1 || data->time_to_die < 0 || 
        data->time_to_eat < 0 || data->time_to_sleep < 0 ||
        (argc == 6 && data->meals_to_eat < 0))
    {
        printf("Error: Invalid argument values\n");
        return 1;
    }

    data->philosophers = malloc(sizeof(t_philosopher) * data->num_philosophers);
    if (!data->philosophers)
    {
        printf("Error: Memory allocation failed\n");
        return 1;
    }

    data->forks = malloc(sizeof(pthread_mutex_t) * data->num_philosophers);
    if (!data->forks)
    {
        printf("Error: Memory allocation failed\n");
        free(data->philosophers);
        return 1;
    }

    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (pthread_mutex_init(&data->forks[i], NULL) != 0)
        {
            printf("Error: Mutex initialization failed\n");
            // Clean up previously initialized mutexes
            for (int j = 0; j < i; j++)
                pthread_mutex_destroy(&data->forks[j]);
            free(data->forks);
            free(data->philosophers);
            return 1;
        }
    }

    if (pthread_mutex_init(&data->write_lock, NULL) != 0)
    {
        printf("Error: Write lock mutex initialization failed\n");
        for (int i = 0; i < data->num_philosophers; i++)
            pthread_mutex_destroy(&data->forks[i]);
        free(data->forks);
        free(data->philosophers);
        return 1;
    }

    data->start_time = get_current_time();
for (int i = 0; i < data->num_philosophers; i++)
{
    data->philosophers[i].id = i + 1;
    data->philosophers[i].times_eaten = 0;
    data->philosophers[i].last_meal_time = data->start_time;
    data->philosophers[i].data = data;
}

    data->simulation_stop = 0;
    pthread_mutex_init(&data->stop_mutex, NULL);

    return 0;
}

int main(int argc, char **argv)
{
    t_data data;
    int i;
    pthread_t monitor_thread;

    if (init_data(&data, argc, argv) != 0)
        return 1;

    usleep(1000); // Small delay before starting threads

    if (pthread_create(&monitor_thread, NULL, monitor_routine, &data) != 0)
    {
        printf("Error creating monitor thread\n");
        cleanup(&data);
        return 1;
    }

    // Create threads for philosophers
    for (i = 0; i < data.num_philosophers; i++)
    {
        if (pthread_create(&data.philosophers[i].thread, NULL, philosopher_routine, &data.philosophers[i]) != 0)
        {
            printf("Error creating thread\n");
            cleanup(&data);
            return 1;
        }
    }

    // Join threads
    for (i = 0; i < data.num_philosophers; i++)
    {
        if (pthread_join(data.philosophers[i].thread, NULL) != 0)
        {
            printf("Error joining thread\n");
            cleanup(&data);
            return 1;
        }
    }

    // Join monitor thread
    if (pthread_join(monitor_thread, NULL) != 0)
    {
        printf("Error joining monitor thread\n");
        cleanup(&data);
        return 1;
    }

    cleanup(&data);
    return 0;
}