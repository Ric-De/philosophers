/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 20:16:45 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/25 20:34:41 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	print_activity(t_philos *philo, int i)
{
	pthread_mutex_lock(&philo->data->print_mutex);
	pthread_mutex_lock(&philo->data->end_mutex);
	if (philo->data->end_flag == false)
	{
		if (i == THINK)
			printf("%ld %d is thinking\n",
				get_time() - philo->data->start_time, philo->id);
		else if (i == LEFT_FORK)
			printf("%ld %d take left fork\n",
				get_time() - philo->data->start_time, philo->id);
		else if (i == RIGHT_FORK)
			printf("%ld %d take right fork\n",
				get_time() - philo->data->start_time, philo->id);
		else if (i == EAT)
			printf("%ld %d is eating\n",
				get_time() - philo->data->start_time, philo->id);
		else if (i == SLEEP)
			printf("%ld %d is sleeping\n",
				get_time() - philo->data->start_time, philo->id);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	pthread_mutex_unlock(&philo->data->print_mutex);
}

int	check_end(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->end_mutex);
	if (philo->data->end_flag)
	{
		pthread_mutex_unlock(&philo->data->end_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->data->end_mutex);
	return (0);
}

void	take_right_fork(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->forks_mutex[philo->right_fork]);
	print_activity(philo, RIGHT_FORK);
}

void	take_left_fork(t_philos *philo)
{
	pthread_mutex_lock(&philo->data->forks_mutex[philo->left_fork]);
	print_activity(philo, LEFT_FORK);
}
