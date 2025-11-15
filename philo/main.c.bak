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

/*  TODO: put more functions here on the go... */

int	main(int argc, char **argv)
{
	t_data	data;
	pthread_t	monitor;
	int	i;
//	long	test_start;
//	long	test_end;

	printf("DEBUG: ========== PHILOSOPHERS START ==========\n");
	
	/* Parse and validate arguments */
	if (!parse_arguments(argc, argv, &data))
		return (1);

	printf("DEBUG: Arguments validated successfully!\n");
	
	/* Initialize all data structures */
	if (!init_data(&data))
	{
		printf("Error: Initialization failed\n");
		
		/* Cleanup any partial initialization */
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}

	/* TEST: Time functions */
//	printf("\nDEBUG: Testing time functions...\n");
//	data.start_time = get_time();
//	printf("DEBUG: Start time set to %ld\n", data.start_time);
//	/* Test ft_usleep accuracy */
//	printf("DEBUG: Testing ft_usleep(500ms)...\n");
//	test_start = get_time();
//	ft_usleep(500);
//	test_end = get_time();
//	printf("DEBUG: Slept for %ld ms (target: 500ms)\n", test_end - test_start);
//	/* Test print_status */
//	printf("\nDEBUG: Testing print_status...\n");
//	print_status(&data.philos[0], "is testing");
//	ft_usleep(100);
//	print_status(&data.philos[1], "is also testing");
//	ft_usleep(100);
//	print_status(&data.philos[2], "is testing too");

	/* Set simulation start time */
	data.start_time = get_time();
	printf("DEBUG: Simulation start time: %ld\n", data.start_time);
	
	/* Initialize all philos last_meal_time to start_time */
	i = 0;
	while (i < data.nb_philos)
	{
		data.philos[i].last_meal_time = data.start_time;
		i++;
	}
	printf("DEBUG: All philos last_meal_time initialized\n");

	/* Create all philosopher threads */
	if (!create_threads(&data))
	{
		printf("Error: Thread creation failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	
	/* Create monitor thread */
	if (pthread_create(&monitor, NULL, monitor_routine, &data) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}
	printf("DEBUG: Monitor thread created\n");
	
	/* Wait for monitor thread (it stops when a 'big head' dies) */
	pthread_join(monitor, NULL);
	printf("DEBUG: Monitor thread joined\n");
	
	/* Wait for all philo threads to finish */
	if (!join_threads(&data))
	{
		printf("Error: Thread joining failed\n");
		cleanup_mutexes(&data);
		cleanup_data(&data);
		return (1);
	}

	/* Cleanup everything before exit */
	cleanup_mutexes(&data);
	cleanup_data(&data);

	printf("DEBUG: ========== PHILOSOPHERS END ==========\n");

	return (0);
	
}
