/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:35:13 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 17:40:50 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Main philosopher routine
** This is what each philosopher thread will execute
** arg: pointer to the philosopher struct (t_philo *)
** Returns NULL when done
*/
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	/* Cast the void pointer back to t_philo pointer */
	philo = (t_philo *)arg;
	printf("DEBUG: Philosopher %d thread started\n", philo->id);
	/* Test: Just print that we're alive */
	print_status(philo, "is thinking");
	ft_usleep(100);
	print_status(philo, "is still alive");
	/* TODO: Add eating/sleeping/thinking loop */
	printf("DEBUG: Philosopher %d thread ending\n", philo->id);
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

	printf("DEBUG: Creating philosopher threads...\n");
	i = 0;
	while (i < data->nb_philos)
	{
		/* Create thread: pass philosopher_routine and pointer to philo */
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create thread for philosopher %d\n",
				data->philos[i].id);
			return (0);
		}
		printf("DEBUG: Thread created for philosopher %d\n",
			data->philos[i].id);
		i++;
	}
	printf("DEBUG: All %d threads created!\n", data->nb_philos);
	return (1);
}

/*
** Wait for all philosopher threads to finish
** data: pointer to main data structure
** Returns 1 on success, 0 on failure
*/
int	join_threads(t_data *data)
{
	int	i;

	printf("DEBUG: Waiting for threads to finish...\n");
	i = 0;
	while (i < data->nb_philos)
	{
		/* pthread_join waits for the thread to complete */
		if (pthread_join(data->philos[i].thread, NULL) != 0)
		{
			printf("Error: Failed to join thread for philosopher %d\n",
				data->philos[i].id);
			return (0);
		}
		printf("DEBUG: Thread %d joined\n", data->philos[i].id);
		i++;
	}
	printf("DEBUG: All threads joined!\n");
	return (1);
}
