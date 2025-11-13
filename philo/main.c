/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:55:01 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 17:28:10 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*  TODO: put more functions here on the go... */

int	main(int argc, char **argv)
{
	t_data	data;
	long	test_start;
	long	test_end;

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
	printf("\nDEBUG: Testing time functions...\n");
	data.start_time = get_time();
	printf("DEBUG: Start time set to %ld\n", data.start_time);
	/* Test ft_usleep accuracy */
	printf("DEBUG: Testing ft_usleep(500ms)...\n");
	test_start = get_time();
	ft_usleep(500);
	test_end = get_time();
	printf("DEBUG: Slept for %ld ms (target: 500ms)\n", test_end - test_start);
	/* Test print_status */
	printf("\nDEBUG: Testing print_status...\n");
	print_status(&data.philos[0], "is testing");
	ft_usleep(100);
	print_status(&data.philos[1], "is also testing");
	ft_usleep(100);
	print_status(&data.philos[2], "is testing too");

	/* TODO: Create philosopher threads */
	/* TODO: Monitor for deaths */
	/* TODO: Join threads */

       	/* Cleanup everything before exit */
	cleanup_mutexes(&data);
	cleanup_data(&data);

	printf("DEBUG: ========== PHILOSOPHERS END ==========\n");

	return (0);
	
}
