/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_input.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pmolzer <pmolzer@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 19:27:12 by pmolzer           #+#    #+#             */
/*   Updated: 2024/08/29 17:32:08 by pmolzer          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Checks if a string contains only numeric characters
int	is_number(char *str)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

// Validates the input arguments for the philosopher program
int	validate_input(int ac, char **av)
{
	int		i;
	long	converted_number;

	if (ac != 5 && ac != 6)
	{
		printf("Error: Invalid number of arguments\n");
		return (1);
	}
	i = 1;
	while (i < ac)
	{
		if (is_number(av[i]) == 0)
		{
			printf("Error: Argument is not a valid number\n");
			return (1);
		}
		converted_number = ft_atol(av[i]);
		if (converted_number > INT_MAX || converted_number <= 0)
		{
			printf("Error: Argument out of range\n");
			return (1);
		}
		i++;
	}
	return (0);
}
