/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 18:12:10 by jaacosta          #+#    #+#             */
/*   Updated: 2025/05/08 19:09:32 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	eat(t_philo *philo)
{
	update_last_meal(philo);
	print_status(philo, "is eating");
	return (precise_usleep
		(philo->shared_data->time_to_eat, philo->shared_data));
}

void	sleep_philo(t_philo *philo)
{
	print_status(philo, "is sleeping");
	return (precise_usleep
		(philo->shared_data->time_to_sleep, philo->shared_data));
}

void	think_philo(t_philo *philo)
{
	print_status(philo, "is thinking");
	ft_usleep(5);
}

void	release_forks(t_philo *philo)
{
	if (philo->id == philo->shared_data->num_philos)
	{
		pthread_mutex_unlock(philo->right_fork);
		pthread_mutex_unlock(philo->left_fork);
	}
	else
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
	}
}
