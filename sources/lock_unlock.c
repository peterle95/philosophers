/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 13:44:35 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/12 18:20:14 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int lock_forks(t_philosopher *philo)
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
}