/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 16:06:42 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/11 16:06:58 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long ft_atol(const char *str)
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