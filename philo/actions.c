/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:51:46 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 19:11:36 by rdellaza         ###   ########.fr       */
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
void	take_forks(t_philo *philo)
{
	/* SPECIAL CASE: Only 1 philosopher = only 1 fork = impossible to eat */
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		/* Wait forever with only 1 fork - will die */
		while (!is_simulation_over(philo->data))
			usleep(100);
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}
	/* Even philosophers: left -> right */
	if (philo->id % 2 == 0)
	{
		/* Lock left fork */
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		/* Check if simulation ended before taking second fork */
		if (is_simulation_over(philo->data))
		{
			pthread_mutex_unlock(philo->left_fork);
			return ;
		}
		/* Lock right fork */
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
	}
	/* Odd philosophers: right -> left */
	else
	{
		/* Lock right fork */
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken a fork");
		/* Check if simulation ended before taking second fork */
		if (is_simulation_over(philo->data))
		{
			pthread_mutex_unlock(philo->right_fork);
			return ;
		}
		/* Lock left fork */
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
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
	
	/* Check if simulation ended while taking forks */
	if (is_simulation_over(philo->data))
	{
		drop_forks(philo);
		return ;
	}

	/* Check if simulation ended while taking forks */
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
** Thinking time helps prevent starvation
** If we have odd number of philos, no special delay needed
** If we have even number, add small delay to prevent lock-step
*/
void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo, "is thinking");
	/* Calculate thinking time to prevent starvation */
	/* If time_to_eat > time_to_sleep, think for the difference */
	if (philo->data->time_to_eat > philo->data->time_to_sleep)
		think_time = (philo->data->time_to_eat - philo->data->time_to_sleep);
	else
		think_time = 0;
	/* Add small delay for even philosophers to prevent lock-step */
	if (think_time == 0 && philo->data->nb_philos % 2 == 0)
		think_time = 1;
	/* Think for calculated time */
	if (think_time > 0)
		ft_usleep(think_time);
}

/*
** Philosopher thinks
** Just prints status (no specific time required)

void	philo_think(t_philo *philo)
{
	print_status(philo, "is thinking");
}
*/
