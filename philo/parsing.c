/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:26:53 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 18:00:54 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Checks if a string contains only digits (and optional + at start)
** Returns 1 if valid, 0 if invalid
*/
int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	if (str[i] == '\0')
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

/*
** Simple atoi implementation
** Converts string to positive integer
** No need to handle negative numbers for this project
*/
int	ft_atoi(const char *str)
{
	int	result;
	int	i;

	result = 0;
	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result);
}

/*
** Parses and validates command line arguments
** argc: argument count (must be 5 or 6)
** argv: argument values
** data: pointer to main data structure to fill
** Returns 1 on success, 0 on error
*/
int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (argc < 5 || argc > 6)
	{
		printf("Error: Invalid number of arguments\n");
		printf("Usage: ./philo nb_philos time_die time_eat time_sleep ");
		printf("[nb_times_must_eat]\n");
		return (0);
	}
	if (!is_valid_number(argv[1]) || !is_valid_number(argv[2])
		|| !is_valid_number(argv[3]) || !is_valid_number(argv[4]))
	{
		printf("Error: All arguments must be positive integers\n");
		return (0);
	}
	if (argc == 6 && !is_valid_number(argv[5]))
	{
		printf("Error: All arguments must be positive integers\n");
		return (0);
	}
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	if (data->nb_philos <= 0 || data->nb_philos > 200)
	{
		printf("Error: Number of philosophers must be 1-200\n");
		return (0);
	}
	if (data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60)
	{
		printf("Error: Time values must be at least 60ms\n");
		return (0);
	}
	if (argc == 6 && data->must_eat_count == 0)
	{
		printf("Error: Number of times to eat must be at least 1\n");
		return (0);
	}
	printf("  - Philosophers: %d\n", data->nb_philos);
	printf("  - Time to die: %d ms\n", data->time_to_die);
	printf("  - Time to eat: %d ms\n", data->time_to_eat);
	printf("  - Time to sleep: %d ms\n", data->time_to_sleep);
	if (data->must_eat_count != -1)
		printf("  - Must eat: %d times\n", data->must_eat_count);
	else
		printf("  - Must eat: unlimited\n");
	return (1);
}