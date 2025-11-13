/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_simulation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:07:55 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:35:50 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *the_philo)
{
	t_philo	*philo;

	philo = the_philo;
	pthread_mutex_lock(&philo->data->start_lock);
	pthread_mutex_unlock(&philo->data->start_lock);
	if (philo->id % 2 == 0)
		usleep(philo->data->time_to_eat / 2 * 1000);
	while (check_simulation_state(philo) == RUNNING)
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

int	create_philos_threads(t_general *data)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&data->start_lock);
	while (i < data->philo_num)
	{
		if (pthread_create(&(data->philos[i].thread_id), NULL, &philo_routine,
				&data->philos[i]) != 0)
		{
			write(2, "ABORT: ERROR WHILE CREATING A THREAD\n", 37);
			data->stop_simulation = 1;
		}
		data->num_threads++;
		i++;
	}
	data->t_start = get_current_time();
	pthread_mutex_unlock(&data->start_lock);
	return (1);
}

void	run_simulation(t_general *data)
{
	create_philos_threads(data);
	supervise_simulation(data);
}
