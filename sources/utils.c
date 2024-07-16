/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:03:05 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:29:31 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long ft_atoll(const char *str)
{
    long long result = 0;
    int sign = 1;
    int i = 0;

    // Handle negative numbers
    if (str[0] == '-')
    {
        sign = -1;
        i++;
    }

    // Convert string to long long
    while (str[i] != '\0')
    {
        if (str[i] < '0' || str[i] > '9')
            return 0; // Invalid input
        result = result * 10 + (str[i] - '0');
        i++;
    }

    return sign * result;
}

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