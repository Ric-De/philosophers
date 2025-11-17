/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:10:17 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/16 17:10:27 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Handle single philosopher case
** Only one fork exists, so philosopher can never eat
*/
static int	handle_one_philo(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	while (!should_philosopher_stop(philo))
		usleep(100);
	pthread_mutex_unlock(philo->left_fork);
	return (0);
}

/*
** Take forks for even-numbered philosophers (left first, then right)
** Returns 1 if got both forks, 0 if should stop
*/
static int	take_forks_even(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	if (should_philosopher_stop(philo))
	{
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	return (1);
}

/*
** Take forks for odd-numbered philosophers (right first, then left)
** Returns 1 if got both forks, 0 if should stop
*/
static int	take_forks_odd(t_philo *philo)
{
	pthread_mutex_lock(philo->right_fork);
	print_status(philo, "has taken a fork");
	if (should_philosopher_stop(philo))
	{
		pthread_mutex_unlock(philo->right_fork);
		return (0);
	}
	pthread_mutex_lock(philo->left_fork);
	print_status(philo, "has taken a fork");
	return (1);
}

/*
** Take forks in the correct order to avoid deadlock
** Even-numbered: LEFT first, then RIGHT
** Odd-numbered: RIGHT first, then LEFT
** Returns 1 if got forks, 0 if should stop
*/
int	take_forks(t_philo *philo)
{
	if (should_philosopher_stop(philo))
		return (0);
	if (philo->data->nb_philos == 1)
		return (handle_one_philo(philo));
	if (philo->id % 2 == 0)
		return (take_forks_even(philo));
	else
		return (take_forks_odd(philo));
}

/*
** Drop both forks (release the mutexes)
** Order doesn't matter for unlocking
*/
void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}
