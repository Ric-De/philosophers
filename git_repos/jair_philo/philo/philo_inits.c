/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_inits.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:39:41 by jaacosta          #+#    #+#             */
/*   Updated: 2025/05/08 19:17:51 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	init_mutexes(t_data *data)
{
	int	i;

	i = 0;
	data->forks = malloc(data->num_philos * sizeof(pthread_mutex_t));
	if (!data->forks)
		printf("Error: Fallo en malloc data.forks");
	i = 0;
	while (i < data->num_philos)
	{
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
			printf("Error: Fallo al crear mutex de tenedor");
		i++;
	}
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
		printf("Error: Fallo al crear mutex de impresion");
	if (pthread_mutex_init(&data->stop_mutex, NULL) != 0)
		printf("Error: Fallo al crear mutex de parada");
}

t_data	init_data(int ac, char **av)
{
	t_data	data;

	data.num_philos = ft_atoi(av[1]);
	data.time_to_die = ft_atoi(av[2]);
	data.time_to_eat = ft_atoi(av[3]);
	data.time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data.must_eat_count = ft_atoi(av[5]);
	else
		data.must_eat_count = -1;
	init_mutexes(&data);
	data.stop_flag = 0;
	data.start_time = get_time();
	return (data);
}

t_philo	*init_philos(t_data *data)
{
	t_philo	*philos;
	int		i;

	philos = malloc(data->num_philos * sizeof(t_philo));
	if (!philos)
		printf("Error: Fallo en malloc philos");
	i = 0;
	while (i < data->num_philos)
	{
		philos[i].id = i + 1;
		philos[i].left_fork = &data->forks[i];
		philos[i].right_fork = &data->forks[(i + 1) % data->num_philos];
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
			printf("Error: Fallo al crear mutex de comida");
		philos[i].last_meal = get_time();
		philos[i].meals_eaten = 0;
		philos[i].shared_data = data;
		i++;
	}
	return (philos);
}
