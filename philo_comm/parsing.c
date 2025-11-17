/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 15:26:53 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:54:48 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** validate_argument_count
** ------------------------
** Validates that the correct number of command line arguments was provided.
** 
** Expected usage:
**   ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_must_eat]
** 
** Parameters:
**   - argc: argument count (includes program name as argv[0])
** 
** Returns:
**   - 1 if argument count is valid (5 or 6 arguments total, meaning 4-5 parameters)
**   - 0 if invalid (prints usage instructions)
** 
** Why 5 or 6?
**   - Program name (argv[0]) = 1
**   - Required parameters = 4 (nb_philos, time_to_die, time_to_eat, time_to_sleep)
**   - Optional parameter = 1 (number_of_times_each_must_eat)
**   Total: 5 or 6
*/
int	validate_argument_count(int argc)
{
	/* Check if we have exactly 4 or 5 parameters (plus program name) */
	if (argc < 5 || argc > 6)
	{
		printf("Error: Wrong number of arguments\n");
		printf("Usage: ./philo nb_philos time_die time_eat time_sleep ");
		printf("[nb_must_eat]\n");
		return (0);
	}
	return (1);
}

/*
** parse_and_validate
** -------------------
** Parses all command line arguments and validates that they are positive integers.
** Fills the data structure with parsed values.
** 
** Validation rules:
**   - All arguments must be valid numbers (digits only)
**   - All arguments must be positive (> 0)
**   - If 5th parameter not provided, must_eat_count is set to -1 (unlimited)
** 
** Parameters:
**   - argv: array of argument strings
**   - data: pointer to data structure to fill with parsed values
** 
** Returns:
**   - 1 if all arguments are valid and successfully parsed
**   - 0 if any argument is invalid (prints error message)
** 
** Fills data structure with:
**   - nb_philos: number of philosophers (and forks)
**   - time_to_die: milliseconds until death from starvation
**   - time_to_eat: milliseconds to eat
**   - time_to_sleep: milliseconds to sleep
**   - must_eat_count: required meals per philosopher (-1 = unlimited)
*/
int	parse_and_validate(char **argv, t_data *data)
{
	int	i;

	/* Start at index 1 (skip program name at argv[0]) */
	i = 1;
	
	/* Loop through all arguments and validate each one */
	while (argv[i])
	{
		/* Check if argument is a valid number (only digits) */
		/* Check if argument is positive (> 0) */
		if (!is_valid_number(argv[i]) || ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Invalid argument '%s'\n", argv[i]);
			return (0);
		}
		i++;
	}
	
	/* Parse and assign all parameters to the data structure */
	data->nb_philos = ft_atoi(argv[1]);        /* Number of philosophers */
	data->time_to_die = ft_atoi(argv[2]);      /* Time until death (ms) */
	data->time_to_eat = ft_atoi(argv[3]);      /* Time to eat (ms) */
	data->time_to_sleep = ft_atoi(argv[4]);    /* Time to sleep (ms) */
	
	/* Parse optional 5th argument (number of times each must eat) */
	if (argv[5])
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;  /* -1 means unlimited (no meal limit) */
	
	return (1);
}

/*
** parse_arguments
** ---------------
** Main parsing function that validates and parses all command line arguments.
** Also prints a summary of the parsed configuration.
** 
** Flow:
** 1. Validates argument count
** 2. Validates and parses each argument value
** 3. Prints configuration summary for user verification
** 
** Parameters:
**   - argc: argument count
**   - argv: argument values
**   - data: pointer to data structure to fill
** 
** Returns:
**   - 1 on success (all arguments valid and parsed)
**   - 0 on failure (invalid argument count or values)
** 
** Configuration summary includes:
**   - Number of philosophers
**   - Time to die (starvation threshold)
**   - Time to eat
**   - Time to sleep
**   - Must eat count (or "unlimited" if not specified)
*/
int	parse_arguments(int argc, char **argv, t_data *data)
{
	/* First, check if we have the correct number of arguments */
	if (!validate_argument_count(argc))
		return (0);
	
	/* Parse and validate all argument values */
	if (!parse_and_validate(argv, data))
		return (0);
	
	/* Print configuration summary for user */
	printf("  - Philosophers: %d\n", data->nb_philos);
	printf("  - Time to die: %d ms\n", data->time_to_die);
	printf("  - Time to eat: %d ms\n", data->time_to_eat);
	printf("  - Time to sleep: %d ms\n", data->time_to_sleep);
	
	/* Print meal requirement */
	if (data->must_eat_count != -1)
		printf("  - Must eat: %d times\n", data->must_eat_count);
	else
		printf("  - Must eat: unlimited\n");
	
	return (1);
}
