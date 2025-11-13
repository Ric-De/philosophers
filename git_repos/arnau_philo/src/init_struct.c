/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_struct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 15:32:36 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/25 20:21:11 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	distribute_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i]->left_fork = (data->philos[i]->id - 1) % data->n_philos;
		data->philos[i]->right_fork = (data->philos[i]->id) % data->n_philos;
		i++;
	}
}

void	create_forks(t_data	*data)
{
	int	i;

	i = 0;
	data->forks_mutex = malloc(data->n_philos * sizeof(pthread_mutex_t));
	if (!data->forks_mutex)
		error_exit(MEMORY_ERROR, data);
	while (i < data->n_philos)
	{
		pthread_mutex_init(&data->forks_mutex[i], NULL);
		i++;
	}
}

void	create_philos(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philos *) * data->n_philos);
	if (!data->philos)
		error_exit(MEMORY_ERROR, data);
	i = 0;
	while (i < data->n_philos)
	{
		data->philos[i] = malloc(sizeof(t_philos));
		if (!data->philos[i])
			error_exit(MEMORY_ERROR, data);
		data->philos[i]->last_time_meal = get_time();
		data->philos[i]->meals_count = 0;
		data->philos[i]->id = i + 1;
		data->philos[i]->data = data;
		i++;
	}
}

void	run_philos(t_data *data)
{
	int			i;
	pthread_t	monitor;

	data->philo = malloc(sizeof(pthread_t) * data->n_philos);
	if (!data->philo)
		error_exit(MEMORY_ERROR, data);
	i = 0;
	while (i < data->n_philos)
	{
		pthread_create(&data->philo[i], NULL, philo_routine, data->philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, monitor_health, data);
	i = 0;
	while (i < data->n_philos)
	{
		pthread_join(data->philo[i], NULL);
		i++;
	}
	pthread_join(monitor, NULL);
}

int	init_struct(int ac, char **av)
{
	t_data		*data;

	data = malloc(sizeof(t_data));
	if (!data)
		return (1);
	parsing(data, ac, av);
	data->end_flag = false;
	data->start_time = get_time();
	pthread_mutex_init(&data->end_mutex, NULL);
	pthread_mutex_init(&data->meal_mutex, NULL);
	pthread_mutex_init(&data->print_mutex, NULL);
	create_philos(data);
	create_forks(data);
	distribute_forks(data);
	run_philos(data);
	free_resources(data);
	return (0);
}
