/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 15:09:41 by pmolzer           #+#    #+#             */
/*   Updated: 2024/07/11 16:57:03 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int is_number(char *str)
{
    int i = 0;
    while (str[i])
    {
        if (str[i] < '0' || str[i] > '9')
            return 0;
        i++;
    }
    return 1;
}

int validate_input(int ac, char **av)
{
    int i;
    long converted_number;

    if (ac != 5 && ac != 6)
    {
        printf("Error: Invalid number of arguments\n");
        return 1;
    }

    i = 1;
    while (i < ac)
    {
        if (!is_number(av[i]))
        {
            printf("Error: Argument %d is not a valid number\n", i);
            return 1;
        }

        converted_number = ft_atol(av[i]);
        if (converted_number > INT_MAX || converted_number <= 0)
        {
            printf("Error: Argument %d is out of range\n", i);
            return 1;
        }

        i++;
    }

    return 0;
}