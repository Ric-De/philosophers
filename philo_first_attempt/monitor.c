/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:27:31 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 15:44:06 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Check if simulation should stop
** Returns 1 if someone died, 0 otherwise
** Protected by mutex to avoid race conditions
*/
int	is_simulation_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->someone_died;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}

/*
** Check if any philosopher has died from starvation
** A philosopher dies if: (current_time - last_meal_time) > time_to_die
** Returns 1 if someone died, 0 if everyone is alive
*/
int	check_death(t_data *data)
{
	int		i;
	long	current_time;
	long	time_since_meal;
	long	last_meal;

	i = 0;
	while (i < data->nb_philos)
	{
		current_time = get_time();
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		time_since_meal = current_time - last_meal;
		if (time_since_meal > data->time_to_die)
		{
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n",
				current_time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

/*
** Check if all philosophers have eaten enough times
** Only relevant when must_eat_count is set (not -1)
** Returns 1 if all philosophers ate >= must_eat_count, 0 otherwise
*/
int	check_all_ate(t_data *data)
{
	int	i;
	int	all_ate_enough;

	if (data->must_eat_count == -1)
		return (0);
	all_ate_enough = 1;
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten < data->must_eat_count)
			all_ate_enough = 0;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		if (!all_ate_enough)
			break ;
		i++;
	}
	if (all_ate_enough)
	{
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	return (0);
}

/*
** Monitor thread routine
** Continuously checks if any philo has died
** Runs until someone dies or all philos finish eating
** arg: pointer to t_data structure
*/
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		if (check_death(data))
		{
			break ;
		}
		if (check_all_ate(data))
			break ;
		usleep(1000);
	}
	return (NULL);
}