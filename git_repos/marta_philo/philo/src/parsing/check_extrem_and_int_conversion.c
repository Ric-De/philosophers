/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_extrem_and_int_conversion.c                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:22:56 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 18:09:33 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_extreme_cases(t_general *data)
{
	if (data->philo_num == 0 || data->time_to_die == 0 || data->time_to_eat == 0
		|| data->time_to_sleep == 0)
	{
		write(2, RED, ft_strlen(RED));
		write(2, "Error: no parameter can be 0\n", 29);
		write(2, END, ft_strlen(END));
		return (0);
	}
	return (1);
}

int	params_conversion_to_int(t_general *data, char **argv)
{
	data->philo_num = ft_atoi(argv[1]);
	data->time_to_die = ft_atoi(argv[2]);
	data->time_to_eat = ft_atoi(argv[3]);
	data->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		data->num_eats_each_philo = ft_atoi(argv[5]);
	else
		data->num_eats_each_philo = -1;
	return (1);
}
