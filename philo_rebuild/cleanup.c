/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:10:48 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/16 17:11:01 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Helper function to destroy philosopher mutexes
** Separated to keep cleanup_mutexes under 25 lines
*/
static void	cleanup_philo_mutexes(t_data *data)
{
	int	i;

	if (!data->philos)
		return ;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->philos[i].meal_mutex);
		pthread_mutex_destroy(&data->philos[i].stop_mutex);
		i++;
	}
}

/*
** Destroy all mutexes
** Must be called before freeing memory
*/
void	cleanup_mutexes(t_data *data)
{
	int	i;

	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
	}
	cleanup_philo_mutexes(data);
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->death_mutex);
}

/*
** Free all allocated memory
** Should be called after cleanup_mutexes()
*/
void	cleanup_data(t_data *data)
{
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
	}
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
	}
}
