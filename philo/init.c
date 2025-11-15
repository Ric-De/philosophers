/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:13:41 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 14:08:52 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Initialize fork mutexes
** Each fork is represented by one mutex
** Returns 1 on success, 0 on failure
*/
int	init_forks(t_data *data)
{
	int	i;

	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
	{
		printf("Error: Failed to allocate memory for the damn forks\n");
		return (0);
	}
	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize fork mutex %d\n", i);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Initialize global mutexes (print and death check)
** Returns 1 on success, 0 on failure
*/
int	init_mutexes(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize print mutex\n");
		return (0);
	}
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize death mutex\n");
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	return (1);
}

/*
** Initialize philosopher structures
** Assigns ID, fork pointers, and links to shared data
** Returns 1 on success, 0 on failure
*/
int	init_philosophers(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		printf("Error: Failed to allocate memory for 'thinking heads'\n");
		return (0);
	}
	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].id = i + 1;
		data->philos[i].meals_eaten = 0;
		data->philos[i].last_meal_time = 0;
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			printf("Error:Failed to init meal mutex philo %d\n", i + 1);
			return (0);
		}
		data->philos[i].left_fork = &data->forks[i];
		data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
		data->philos[i].data = data;
		printf("(left_fork=%d, right_fork=%d)\n", i, (i + 1) % data->nb_philos);
		i++;
	}
	return (1);
}

/*
** Master initialization function
** Calls all sub-initialization functions in order
** Returns 1 on success, 0 on failure
*/
int	init_data(t_data *data)
{
	data->someone_died = 0;
	data->start_time = 0;
	data->philos = NULL;
	data->forks = NULL;
	if (!init_mutexes(data))
		return (0);
	if (!init_forks(data))
	{
		return (0);
	}
	if (!init_philosophers(data))
	{
		return (0);
	}
	return (1);
}
