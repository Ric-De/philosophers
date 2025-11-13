/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 01:36:46 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:09:46 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_2_last_mutex(t_general *data)
{
	pthread_mutex_destroy(&data->start_lock);
	pthread_mutex_destroy(&data->write_lock);
}

void	free_3_last_mutex(t_general *data)
{
	pthread_mutex_destroy(&data->start_lock);
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->life_checker_lock);
}

void	free_since_last_meal_function(t_general *data)
{
	int	i;

	i = 0;
	free_3_last_mutex(data);
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->last_meal_array[i]);
		i++;
	}
	free(data->last_meal_array);
}

void	free_since_num_meals_function(t_general *data)
{
	int	i;

	i = 0;
	free_since_last_meal_function(data);
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->num_meals_array[i]);
		i++;
	}
	free(data->num_meals_array);
}
