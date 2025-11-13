/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   controller.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 22:41:57 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 13:01:18 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_simulation_state(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->life_checker_lock);
	if (!philo->data->stop_simulation)
	{
		pthread_mutex_unlock(&philo->data->life_checker_lock);
		return (RUNNING);
	}
	pthread_mutex_unlock(&philo->data->life_checker_lock);
	return (STOPPED);
}

int	all_meals_eaten(t_general *data, int num_of_philos_eaten_enough)
{
	if (data->philo_num == num_of_philos_eaten_enough)
	{
		pthread_mutex_lock(&data->life_checker_lock);
		data->stop_simulation = 1;
		pthread_mutex_unlock(&data->life_checker_lock);
		pthread_mutex_lock(&data->write_lock);
		printf(RED "We are full of eating so much spaghetti 🥴💥\n" END);
		pthread_mutex_unlock(&data->write_lock);
		return (1);
	}
	return (0);
}

int supervise_number_meals (t_general *data, int i, int	*num_of_philos_eaten_enough)
{
	
	pthread_mutex_lock(data->philos[i].n_meals);
	if (data->philos[i].num_meals >= data->num_eats_each_philo)
		(*num_of_philos_eaten_enough)++;
	pthread_mutex_unlock(data->philos[i].n_meals);
	if (all_meals_eaten(data, *num_of_philos_eaten_enough))
		return (0);

	return (1);
}

void	kill_philo(t_general *data, t_philo *philo)
{
	unsigned int	current_time;

	current_time = get_simulation_time(data);
	pthread_mutex_lock(&data->life_checker_lock);
	data->stop_simulation = 1;
	pthread_mutex_unlock(&data->life_checker_lock);
	pthread_mutex_lock(&data->write_lock);
	printf(RED "[%d]  %d %s\n" END, current_time, philo->id, DIED);
	pthread_mutex_unlock(&data->write_lock);
}

void	supervise_simulation(t_general *data)
{
	int	i;
	int	num_of_philos_eaten_enough;

	num_of_philos_eaten_enough = 0;

	i = 0;
	while (!data->stop_simulation)
	{
		if (i == data->philo_num)
		{
			i = 0;
			num_of_philos_eaten_enough = 0;
		}
		if (data->num_eats_each_philo != -1)
		{
			if (supervise_number_meals (data, i, &num_of_philos_eaten_enough) == 0)
				break ;
		}
		pthread_mutex_lock(data->philos[i].l_meal);
		if (get_simulation_time(data)
			- data->philos[i].last_meal >= data->time_to_die)
			kill_philo(data, &data->philos[i]);
		pthread_mutex_unlock(data->philos[i].l_meal);
		i++;
	}
}
