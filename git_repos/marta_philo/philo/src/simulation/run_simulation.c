/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:07:55 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/12 11:11:17 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(t_philo *philo)
{
	pthread_mutex_lock(&philo->data->start_lock);
	pthread_mutex_unlock(&philo->data->start_lock);
	if (philo->id % 2 != 0)
		suspend(philo, philo->data->time_to_eat / 2);
	while (42)
	{
		if (eat(philo) == 0)
			break ;
		if (sleep_nap(philo) == 0)
			break ;
		if (think(philo) == 0)
			break ;
	}
	return (NULL);
}

void	create_philos_threads(t_general *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->start_lock);
	while (i < data->philo_num)
	{
		if (pthread_create(&(data->philos[i].thread_id), NULL,
				(void *)philo_routine, &data->philos[i]) != 0)
		{
			write(2, RED, ft_strlen(RED));
			write(2, "ABORT: ERROR WHILE CREATING A THREAD\n", 38);
			write(2, "Too many philosophers, ", 23);
			write(2, "not enough spaghetti for everyone\n", 34);
			write(2, END, ft_strlen(END));
			data->stop_simulation = 1;
			break ;
		}
		data->num_threads++;
		i++;
	}
	data->t_start = get_current_time();
	pthread_mutex_unlock(&data->start_lock);
}

void	run_simulation(t_general *data)
{
	create_philos_threads(data);
	supervise_simulation(data);
}
