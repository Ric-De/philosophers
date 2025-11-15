/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:51:46 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 15:36:05 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Take forks in the correct order to avoid deadlock
** SPECIAL CASE: If only 1 philosopher, they can never eat (only 1 fork exists)
** DEADLOCK PREVENTION STRATEGY:
** - Even-numbered philosophers: take LEFT fork first, then RIGHT
** - Odd-numbered philosophers: take RIGHT fork first, then LEFT
** This ensures at least one philosopher can always eat
*/
int	take_forks(t_philo *philo)
{
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		while (!is_simulation_over(philo->data))
			usleep(100);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		if (is_simulation_over(philo->data))
		{
			pthread_mutex_unlock(philo->left_fork);
			return (0);
		}
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		if (is_simulation_over(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			return (0);
		}
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
	}
	return (1);
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

/*
** Philosopher eats RANGO TIME
** - Takes forks
** - Updates last meal time (IMPORTANT for death detection!)
** - Prints eating status
** - Sleeps for time_to_eat
** - Increments meal count
** - Drops forks
*/
void	philo_eat(t_philo *philo)
{
	int	got_forks;
	
	got_forks = take_forks(philo);
	if (!got_forks)
		return ;
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	print_status(philo, "is eating");
	ft_usleep(philo->data->time_to_eat);
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	drop_forks(philo);
}

/*
** Philosopher sleeps
** Just prints status and sleeps for time_to_sleep
*/
void	philo_sleep(t_philo *philo)
{
	print_status(philo, "is sleeping");
	ft_usleep(philo->data->time_to_sleep);
}

/*
** Philosopher thinks
** Thinking time helps prevent starvation
** Keep it minimal to maximize eating opportunities
*/
void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	if (philo->data->nb_philos % 2 == 0)
		think_time = 1;
	else
		think_time = 0;
	if (think_time > 0)
		ft_usleep(think_time);
}