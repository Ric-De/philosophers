/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:51:46 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 18:18:04 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Take forks in the correct order to avoid deadlock
** DEADLOCK PREVENTION STRATEGY:
** - Even-numbered philos: take LEFT fork first, then RIGHT
** - Odd-numbered philos: take RIGHT fork first, then LEFT
** This ensures at least one philo can always eat
*/
void	take_forks(t_philo *philo)
{
	/* Even philos: left -> right */
	if (philo->id % 2 == 0)
	{
		/* Lock left fork */
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		printf("DEBUG: Philo %d took LEFT fork\n", philo->id);
		/* Lock right fork */
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		printf("DEBUG: Philo %d took RIGHT fork\n", philo->id);
	}
	/* Odd philos: right -> left */
	else
	{
		/* Lock right fork */
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		printf("DEBUG: Philo %d took RIGHT fork\n", philo->id);
		/* Lock left fork */
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		printf("DEBUG: Philo %d took LEFT fork\n", philo->id);
	}
}

/*
** Drop both forks (release the mutexes)
** Order doesn't matter for unlocking
*/
void	drop_forks(t_philo *philo)
{
	/* Unlock left fork */
	pthread_mutex_unlock(philo->left_fork);
	printf("DEBUG: Philo %d dropped LEFT fork\n", philo->id);
	/* Unlock right fork */
	pthread_mutex_unlock(philo->right_fork);
	printf("DEBUG: Philo %d dropped RIGHT fork\n", philo->id);
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
	/* Take both forks */
	take_forks(philo);
	/* Update last meal time - CRITICAL for death detection */
	/* TODO: Protect this with a mutex later */
	philo->last_meal_time = get_time();
	printf("DEBUG: Philo %d last_meal_time updated to %ld\n",
		philo->id, philo->last_meal_time);
	/* Print eating status */
	print_status(philo, "is eating");
	/* Eat for the specified time */
	ft_usleep(philo->data->time_to_eat);
	/* Increment meal counter */
	philo->meals_eaten++;
	printf("DEBUG: Philo %d meteu rango %d times\n",
		philo->id, philo->meals_eaten);
	/* Drop forks */
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
** Just prints status (no specific time required)
*/
void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}
