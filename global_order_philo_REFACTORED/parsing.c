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
** Validate argument count
*/
int	validate_argument_count(int argc)
{
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
** Parse and validate all arguments
*/
int	parse_and_validate(char **argv, t_data *data)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (!is_valid_number(argv[i]) || ft_atoi(argv[i]) <= 0)
		{
			printf("Error: Invalid argument '%s'\n", argv[i]);
			return (0);
		}
		i++;
	}
	data->nb_philos = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->must_eat_count = ft_atoi(argv[5]);
	else
		data->must_eat_count = -1;
	return (1);
}

/*
** Parses and validates command line arguments
** argc: argument count (must be 5 or 6)
** argv: argument values
** data: pointer to main data structure to fill
** Returns 1 on success, 0 on error
**
** Parse command line arguments
*/
int	parse_arguments(int argc, char **argv, t_data *data)
{
	if (!validate_argument_count(argc))
		return (0);
	if (!parse_and_validate(argv, data))
		return (0);
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
