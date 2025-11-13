/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 12:08:32 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:29:08 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	free_since_build_philos_function(t_general *data)
{
	free_since_num_meals_function(data);
	free(data->philos);
}

void	free_all_data(t_general *data)
{
	int	i;

	i = 0;
	free_since_build_philos_function(data);
	while (i < data->philo_num)
	{
		pthread_mutex_destroy(&data->forks_array[i]);
		i++;
	}
	free(data->forks_array);
}
