/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:36:58 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/24 15:37:02 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	ft_is_digit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
		{
			if (str[i] != '+' && str[i] != '-')
				return (0);
		}
		i++;
	}
	return (1);
}

void	check_syntax(t_data *data, char **av)
{
	int	i;

	i = 0;
	while (av[++i])
	{
		if (!ft_is_digit(av[i]))
			error_exit(INPUT_ERROR, data);
	}
}

int	ft_atoi(char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	if (*str == '+' || *str == '-')
	{
		if (*str == '-')
			sign *= -1;
		str++;
	}
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + *str - '0';
		str++;
	}
	return (result * sign);
}

void	parsing(t_data *data, int ac, char **av)
{
	check_syntax(data, av);
	data->n_philos = ft_atoi(av[1]);
	data->t_die = ft_atoi(av[2]);
	data->t_eat = ft_atoi(av[3]);
	data->t_sleep = ft_atoi(av[4]);
	data->meals_flag = 0;
	if (ac == 6)
	{
		data->meals = ft_atoi(av[5]);
		data->meals_flag = 1;
	}
	else
		data->meals = 0;
	check_input(data, data->meals_flag);
}
