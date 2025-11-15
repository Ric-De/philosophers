/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:55:01 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 19:26:24 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

int	main(int argc, char **argv)
{
	t_data	data;
	pthread_t	monitor;
	int	i;

	if (!parse_arguments(argc, argv, &data))
		return (1);
	if (!init_data(&data))
	{
		printf("Error: Initialization failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	data.start_time = get_time();
	i = 0;
	while (i < data.nb_philos)
	{
		data.philos[i].last_meal_time = data.start_time;
		i++;
	}
	if (!create_threads(&data))
	{
		printf("Error: Thread creation failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	pthread_join(monitor, NULL);
	if (!join_threads(&data))
	{
		printf("Error: Thread joining failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	cleanup_mutexes(&data);
	cleanup_data(&data);
	return (0);
}