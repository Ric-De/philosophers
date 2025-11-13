/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:35:13 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 18:02:45 by rdellaza         ###   ########.fr       */
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
	printf("DEBUG: Philo %d thread started\n", philo->id);
	
	/* Test thinking */
	print_status(philo, "is thinking");
	/* Small delay so threads don't all start at exact same time */
	ft_usleep(50);

	/* TEST: Take forks */
	take_forks(philo);
	/* TEST: Hold forks briefly, then drop them */
	print_status(philo, "is holding forks (testing)");
	ft_usleep(100);
	/* TEST: Drop forks */
	drop_forks(philo);
	/* TODO: Add eating action */
	/* TODO: Add sleeping action */
	/* TODO: Add main loop for continuous eating/sleeping/thinking */

	printf("DEBUG: Philo %d thread ending\n", philo->id);
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

	printf("DEBUG: Creating philo threads...\n");
	i = 0;
	while (i < data->nb_philos)
	{
		/* Create thread: pass philosopher_routine and pointer to philo */
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		printf("DEBUG: Thread created for philo %d\n",
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
			printf("Error: Failed to join thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		printf("DEBUG: Thread %d joined\n", data->philos[i].id);
		i++;
	}
	printf("DEBUG: All threads joined!\n");
	return (1);
}
