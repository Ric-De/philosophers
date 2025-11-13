/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:04:36 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/26 19:12:05 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	eat_meal(t_philos *philo)
{
	print_activity(philo, EAT);
	pthread_mutex_lock(&philo->data->meal_mutex);
	philo->last_time_meal = get_time();
	philo->meals_count++;
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_usleep(philo->data->t_eat);
	pthread_mutex_unlock(&philo->data->forks_mutex[philo->right_fork]);
	pthread_mutex_unlock(&philo->data->forks_mutex[philo->left_fork]);
}

void	take_forks(t_philos *philo)
{
	if (philo->id % 2 == 0)
	{
		take_left_fork(philo);
		take_right_fork(philo);
	}
	else
	{
		take_right_fork(philo);
		take_left_fork(philo);
	}
}

void	get_sleep(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	print_activity(philo, SLEEP);
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_usleep(philo->data->t_sleep);
}

void	think(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->meal_mutex);
	print_activity(philo, THINK);
	pthread_mutex_unlock(&philo->data->meal_mutex);
	ft_usleep(5);
}

void	*philo_routine(void *arg)
{
	t_philos	*philo;

	philo = (t_philos *)arg;
	if (philo->data->n_philos == 1)
	{
		print_activity(philo, RIGHT_FORK);
		ft_usleep(philo->data->t_die);
		return (NULL);
	}
	ph_delay(philo->data->start_time);
	while (1)
	{
		if (check_end(philo))
			break ;
		take_forks(philo);
		eat_meal(philo);
		if (philo->meals_count == philo->data->meals)
			break ;
		get_sleep(philo);
		think(philo);
	}
	return (NULL);
}
