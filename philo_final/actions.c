/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:51:46 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:00:17 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Take forks using global ordering strategy
** Single philosopher case: wait until simulation ends
** Normal case: use helper functions for fork selection and locking
*/
int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		while (!is_simulation_over(philo->data))
			usleep(100);
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	select_fork_order(philo, &first_fork, &second_fork);
	if (!lock_first_fork(philo, first_fork))
		return (0);
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
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
	if (philo->data->nb_philos % 2 != 0)
		think_time = 1;
	else
		think_time = 0;
	if (think_time > 0)
		ft_usleep(think_time);
}
