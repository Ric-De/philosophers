/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:55:01 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 16:20:07 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*  TODO: put more functions here on the go... */

int	main(int argc, char **argv)
{
	t_data	data;

	printf("DEBUG: ========== PHILOSOPHERS START ==========\n");
	/* Parse and validate arguments */
	if (!parse_arguments(argc, argv, &data))
		return (1);

	printf("DEBUG: Arguments validated successfully!\n");
	/* Initialize all data structures */
	if (!init_data(&data))
	{
		printf("Error: Initialization failed\n");
		/* TODO: Cleanup any partial initialization */
		return (1);
	}

	/* TODO: Create philosopher threads */
	/* TODO: Monitor for deaths */
	/* TODO: Join threads and cleanup */

	printf("DEBUG: ========== PHILOSOPHERS END ==========\n");

	return (0);
	
}
