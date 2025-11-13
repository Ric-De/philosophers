/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 22:38:28 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:58:01 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

int	think(t_philo *philo)
{
	return (print_state(philo, THINKING_STATE));
}

int	sleep_nap (t_philo *philo)
{
	int	status;

	status = print_state(philo, SLEEPING_STATE);
	if (status == RUNNING)
		usleep(philo->data->time_to_sleep * 1000); 
	return (status);
}

void	unlock_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
}

int	take_forks(t_philo *philo)
{
	int	status;

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		status = print_state(philo, TAKEN_FORK_STATE);
		if (status == STOPPED || philo->right_fork == philo->left_fork)
			return (pthread_mutex_unlock(philo->left_fork), 0);
		pthread_mutex_lock(philo->right_fork);
		status = print_state(philo, TAKEN_FORK_STATE);
		if (status == STOPPED)
			return (unlock_forks(philo), 0);
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		status = print_state(philo, TAKEN_FORK_STATE);
		if (status == STOPPED || philo->right_fork == philo->left_fork)
			return (pthread_mutex_unlock(philo->right_fork), 0);
		pthread_mutex_lock(philo->left_fork);
		status = print_state(philo, TAKEN_FORK_STATE);
		if (status == STOPPED)
			return (unlock_forks(philo), 0);
	}
	return (status);
}

int	eat(t_philo *philo)
{
	if (take_forks(philo) == STOPPED)
		return (0);
	if (print_state(philo, EATING_STATE) == STOPPED)
		return (unlock_forks(philo), 0);
	pthread_mutex_lock(philo->l_meal);
	philo->last_meal = get_simulation_time(philo->data);
	pthread_mutex_unlock(philo->l_meal);
	pthread_mutex_lock(philo->n_meals);
	philo->num_meals++;
	pthread_mutex_unlock(philo->n_meals);
	usleep(philo->data->time_to_eat * 1000);
	unlock_forks(philo);
	return (1);
}





