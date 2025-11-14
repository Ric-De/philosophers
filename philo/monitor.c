/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:27:31 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 14:16:29 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Check if simulation should stop
** Returns 1 if someone died, 0 otherwise
** Protected by mutex to avoid race conditions
*/
int	is_simulation_over(t_data *data)
{
	int	result;

	pthread_mutex_lock(&data->death_mutex);
	result = data->someone_died;
	pthread_mutex_unlock(&data->death_mutex);
	return (result);
}

/*
** Check if any philosopher has died from starvation
** A philosopher dies if: (current_time - last_meal_time) > time_to_die
** Returns 1 if someone died, 0 if everyone is alive
*/
int	check_death(t_data *data)
{
	int		i;
	long	current_time;
	long	time_since_meal;
	long	last_meal;

	i = 0;
	while (i < data->nb_philos)
	{
		current_time = get_time();
		/* Read last_meal_time with mutex protection */
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		last_meal = data->philos[i].last_meal_time;
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		
		/* Calculate how long since this philosopher last ate */
		time_since_meal = current_time - last_meal;
		
		/* Check if philosopher has starved */
		if (time_since_meal > data->time_to_die)
		{
			/* Lock print mutex to print death message */
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n",
				current_time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			/* Set death flag */
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			printf("DEBUG: Monitor detected death of philo %d\n",
				data->philos[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}

/*			OLDest
** Check if any philo has died from starvation
** A philosopher dies if: (current_time - last_meal_time) > time_to_die
** Returns 1 if someone died, 0 if everyone is alive

int	check_death(t_data *data)
{
	int		i;
	long	current_time;
	long	time_since_meal;

	i = 0;
	while (i < data->nb_philos)
	{
		current_time = get_time();
		// Calculate how long since this philo last ate
		// TODO: Protect last_meal_time with mutex
		time_since_meal = current_time - data->philos[i].last_meal_time;
				
		// Check if philosopher has starved
		if (time_since_meal > data->time_to_die)
		{
			// Lock print mutex to print death message
			pthread_mutex_lock(&data->print_mutex);
			printf("%ld %d died\n",
				current_time - data->start_time, data->philos[i].id);
			pthread_mutex_unlock(&data->print_mutex);
			// Set death flag
			pthread_mutex_lock(&data->death_mutex);
			data->someone_died = 1;
			pthread_mutex_unlock(&data->death_mutex);
			printf("DEBUG: Monitor detected death of philo %d\n",
				data->philos[i].id);
			return (1);
		}
		i++;
	}
	return (0);
}
*/

/*
** Monitor thread routine
** Continuously checks if any philo has died
** Runs until someone dies or all philos finish eating
** arg: pointer to t_data structure
*/
void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	printf("DEBUG: Monitor thread started\n");
	/* Keep monitoring until someone dies */
	while (1)
	{
		/* Check if anyone died */
		if (check_death(data))
		{
			printf("DEBUG: Monitor detected death, stopping\n");
			break ;
		}
		/* TODO: Check if all philosophers ate enough times */
		/* Small sleep to avoid burning CPU */
		/* Check frequently (every 1ms) to meet the 10ms death detection rule */
		usleep(1000);
	}
	printf("DEBUG: Monitor thread ending\n");
	return (NULL);
}
