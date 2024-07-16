/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 15:36:15 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/16 19:26:05 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int argc, char **argv)
{
    t_data data;
    int i;
    pthread_t monitor_thread;

    if(validate_input(argc, argv) != 0)
        return(EXIT_FAILURE);

    if (init_data(&data, argc, argv) != 0)
        return 1;

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

    // Small delay to ensure all threads have started
    usleep(1000);

    // Set the start time after all threads have been created
    data.start_time = get_current_time();

    // Create monitor thread after all philosopher threads
    if (pthread_create(&monitor_thread, NULL, monitor_routine, &data) != 0)
    {
        printf("Error creating monitor thread\n");
        cleanup(&data);
        return 1;
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