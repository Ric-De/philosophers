/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_logic.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:33:17 by jaacosta          #+#    #+#             */
/*   Updated: 2025/04/18 20:19:24 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	print_status(t_philo *philo, char *str)
{
	pthread_mutex_lock(&philo->shared_data->print_mutex);
	if (!check_stop(philo->shared_data))
	{
		printf("%lld %d %s\n", get_time() - philo->shared_data->start_time,
			philo->id, str);
	}
	pthread_mutex_unlock(&philo->shared_data->print_mutex);
}

void	take_forks(t_philo *philo)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken: left fork");
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken: right fork");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_status(philo, "has taken: right fork");
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken: left fork");
	}
}

void	*philosopher(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->shared_data->num_philos == 1)
	{
		print_status(philo, "has taken: right fork");
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(5);
	while (!check_stop(philo->shared_data))
	{
		if (philo->shared_data->must_eat_count != -1
			&& philo->meals_eaten >= philo->shared_data->must_eat_count)
			break ;
		take_forks(philo);
		eat(philo);
		release_forks(philo);
		sleep_philo(philo);
		think_philo(philo);
	}
	return (NULL);
}

int	check_deat_or_meals(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		if ((get_time() - get_last_meal(&data->philos[i])
				> data->time_to_die))
		{
			print_status(&data->philos[i], " died");
			set_stop(data);
			return (1);
		}
		i++;
	}
	if (check_meals(data))
	{
		set_stop(data);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	if (data->num_philos % 2 == 0)
		usleep(5000);
	while (!check_stop(data))
	{
		if (check_deat_or_meals(data))
			return (NULL);
	}
	return (NULL);
}
