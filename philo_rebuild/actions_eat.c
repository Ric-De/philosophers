/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_eat.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:11:51 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/16 17:12:01 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Philosopher eats
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
