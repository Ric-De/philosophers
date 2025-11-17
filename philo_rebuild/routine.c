/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:09:21 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/16 17:09:46 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Main philosopher routine
** This is what each philosopher thread will execute
** The cycle: EAT -> SLEEP -> THINK -> repeat
** Stops when personal stop flag is set
** arg: pointer to the philosopher struct (t_philo *)
** Returns NULL when done
*/
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->data->nb_philos % 2 == 0)
	{
		if (philo->id % 2 == 0)
			ft_usleep(philo->data->time_to_eat - 10);
	}
	while (!should_philosopher_stop(philo))
	{
		philo_eat(philo);
		if (should_philosopher_stop(philo))
			break ;
		philo_sleep(philo);
		if (should_philosopher_stop(philo))
			break ;
		philo_think(philo);
	}
	return (NULL);
}

/*
** Create all philosopher threads
** data: pointer to main data structure
** Returns 1 on success, 0 on failure
*/
int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Wait for all philo threads to finish
** data: pointer to main data structure
** Returns 1 on success, 0 on failure
*/
int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (pthread_join(data->philos[i].thread, NULL) != 0)
		{
			printf("Error: Failed to join thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		i++;
	}
	return (1);
}
