/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:36:15 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 20:25:59 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int create_philosopher_threads(t_data *data)
{
    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (pthread_create(&data->philosophers[i].thread, NULL, philosopher_routine, &data->philosophers[i]) != 0)
        {
            printf("Error creating thread\n");
            return 1;
        }
    }
    return 0;
}

int create_monitor_thread(t_data *data, pthread_t *monitor_thread)
{
    if (pthread_create(monitor_thread, NULL, monitor_routine, data) != 0)
    {
        printf("Error creating monitor thread\n");
        return 1;
    }
    return 0;
}

int join_philosopher_threads(t_data *data)
{
    for (int i = 0; i < data->num_philosophers; i++)
    {
        if (pthread_join(data->philosophers[i].thread, NULL) != 0)
        {
            printf("Error joining thread\n");
            return 1;
        }
    }
    return 0;
}
int create_and_run_threads(t_data *data)
{
    pthread_t monitor_thread;

    if (create_philosopher_threads(data) != 0)
        return 1;
    usleep(1000);
    data->start_time = get_current_time();
    if (create_monitor_thread(data, &monitor_thread) != 0)
        return 1;
    if (join_philosopher_threads(data) != 0)
        return 1;
    if (pthread_join(monitor_thread, NULL) != 0)
    {
        printf("Error joining monitor thread\n");
        return 1;
    }
    return 0;
}

int run_simulation(int argc, char **argv)
{
    t_data data;

    if (init_data(&data, argc, argv) != 0)
        return 1;

    if (create_and_run_threads(&data) != 0)
    {
        cleanup(&data);
        return 1;
    }

    cleanup(&data);
    return 0;
}

int main(int argc, char **argv)
{
    if (validate_input(argc, argv) != 0)
        return EXIT_FAILURE;

    return run_simulation(argc, argv) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}