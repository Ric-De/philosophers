/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:20:27 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 18:36:28 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Main philosopher routine
** This is what each philosopher thread will execute
** The cycle: THINK -> EAT -> SLEEP -> repeat
** Stops when someone dies
** arg: pointer to the philosopher struct (t_philo *)
** Returns NULL when done
*/
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	/* Cast the void pointer back to t_philo pointer */
	philo = (t_philo *)arg;
	printf("DEBUG: Philo %d thread started\n", philo->id);
	
	/* Initialize last meal time to simulation start */
	philo->last_meal_time = philo->data->start_time;
	
	/* Small stagger: even philos start slightly delayed */
	/* This helps prevent all philos grabbing forks simultaneously */
	if (philo->id % 2 == 0)
		ft_usleep(50);
	
	/* Main loop: keep cycling until someone dies */
	while (!is_simulation_over(philo->data))
	{
		philo_think(philo);
		/* Check again before eating (philo might die while thinking) */
		if (is_simulation_over(philo->data))
			break ;
		philo_eat(philo);
		if (is_simulation_over(philo->data))
			break ;
		philo_sleep(philo);
	}
	printf("DEBUG: Philo %d detected end of simulation, exiting\n",
		philo->id);
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
		i++;
	}
	printf("DEBUG: All %d threads created!\n", data->nb_philos);
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
		i++;
	}
	printf("DEBUG: All threads joined!\n");
	return (1);
}
