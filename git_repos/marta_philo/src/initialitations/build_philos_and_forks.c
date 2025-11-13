/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_philos_and_forks.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 21:35:23 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:23:50 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	build_forks_array(t_general *data)
{
	int	i;

	data->forks_array = malloc(sizeof(pthread_mutex_t) * (data->philo_num));
	if (!data->forks_array)
	{
		write(2, "ABORT: Malloc Failed\n", 21);
		free_since_build_philos_function(data);
		return (0); // pues saldria limpio
	}
	i = 0;
	while (i < data->philo_num)
	{
		if (pthread_mutex_init(&data->forks_array[i], NULL) != 0)
		{
			// REVISAR
			write(2, "ABORT: Error while creatin a Mutex\n", 35);
			free_since_build_philos_function(data);
			while (--i >= 0)
				pthread_mutex_destroy(&data->forks_array[i]);
			free(data->forks_array);
			return (0);
		}
		i++;
	}
	return (1);
}

int	build_philos_array(t_general *data)
{
	t_philo	*philos;

	philos = malloc(sizeof(t_philo) * (data->philo_num));
	if (!philos)
	{
		// REVISAR
		write(2, "ABORT: Malloc Failed\n", 21);
		free_since_num_meals_function(data);
		return (0);
	}
	data->philos = philos;
	return (1);
}

int	build_philos_and_forks(t_general *data)
{
	if (build_philos_array(data) == 0)
		return (0);
	if (build_forks_array(data) == 0)
		return (0);
	return (1);
}
