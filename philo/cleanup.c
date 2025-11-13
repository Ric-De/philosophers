/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:34:52 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 16:36:15 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Destroy all mutexes
** Must be called before freeing memory
*/
void	cleanup_mutexes(t_data *data)
{
	int	i;

	printf("DEBUG: Destroying mutexes...\n");
	/* Destroy fork mutexes */
	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
		printf("DEBUG: %d fork mutexes destroyed\n", data->nb_philos);
	}
	/* Destroy print mutex */
	pthread_mutex_destroy(&data->print_mutex);
	printf("DEBUG: Print mutex destroyed\n");
	/* Destroy death mutex */
	pthread_mutex_destroy(&data->death_mutex);
	printf("DEBUG: Death mutex destroyed\n");
}

/*
** Free all allocated memory
** Should be called after cleanup_mutexes()
*/
void	cleanup_data(t_data *data)
{
	printf("DEBUG: Freeing allocated memory...\n");
	/* Free forks array */
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;
		printf("DEBUG: Forks array freed\n");
	}
	/* Free philosophers array */
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;
		printf("DEBUG: Philosophers array freed\n");
	}
	printf("DEBUG: Cleanup complete!\n");
}
