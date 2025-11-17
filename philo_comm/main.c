/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:55:01 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:54:34 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** initialize_simulation
** ---------------------
** Prepares the entire simulation before starting threads.
** 
** Flow:
** 1. Parses and validates command line arguments (number of philosophers,
**    time constraints, etc.)
** 2. Initializes all data structures (mutexes, forks, philosophers)
** 
** Parameters:
**   - argc: argument count from command line
**   - argv: argument values (program params)
**   - data: pointer to main data structure that will be filled
** 
** Returns:
**   - 1 on success (everything initialized correctly)
**   - 0 on failure (invalid args or initialization error)
** 
** On failure, this function cleans up any partially initialized resources
** to prevent memory leaks and ensure proper mutex destruction.
*/
int	initialize_simulation(int argc, char **argv, t_data *data)
{
	/* Parse command line arguments and validate them */
	if (!parse_arguments(argc, argv, data))
		return (0);
	
	/* Initialize all data structures (mutexes, forks, philosophers) */
	if (!init_data(data))
	{
		printf("Error: Initialization failed\n");
		/* Clean up any partially initialized resources */
		cleanup_mutexes(data);
		cleanup_data(data);
		return (0);
	}
	return (1);
}

/*
** run_simulation
** --------------
** Executes the main simulation after initialization is complete.
** 
** Flow:
** 1. Records simulation start time (used for all timestamps)
** 2. Sets initial last_meal_time for all philosophers to start_time
**    (important: prevents false death detection at the very beginning)
** 3. Creates all philosopher threads (each runs philosopher_routine)
** 4. Creates monitor thread (continuously checks for deaths/completion)
** 5. Waits for monitor to finish (blocks until simulation ends)
** 6. Joins all philosopher threads (waits for them to clean up)
** 
** Parameters:
**   - data: pointer to initialized data structure
** 
** Returns:
**   - 1 on success
**   - 0 on thread creation/joining failure
** 
** The monitor thread determines when simulation ends by either:
**   - Detecting a philosopher death (starvation)
**   - Confirming all philosophers ate required number of meals
*/
int	run_simulation(t_data *data)
{
	pthread_t	monitor;
	int			i;

	/* Record the exact start time of the simulation */
	data->start_time = get_time();
	
	/* Initialize each philosopher's last meal time to start time
	** This prevents false death detection at the very beginning
	** since time_since_last_meal would be huge if last_meal_time was 0 */
	i = 0;
	while (i < data->nb_philos)
	{
		data->philos[i].last_meal_time = data->start_time;
		i++;
	}
	
	/* Create all philosopher threads
	** Each thread will execute philosopher_routine() independently */
	if (!create_threads(data))
	{
		printf("Error: Thread creation failed\n");
		return (0);
	}
	
	/* Create the monitor thread
	** This thread continuously checks if anyone died or if everyone finished eating */
	if (pthread_create(&monitor, NULL, monitor_routine, data) != 0)
	{
		printf("Error: Monitor thread creation failed\n");
		return (0);
	}
	
	/* Wait for the monitor thread to finish
	** The monitor decides when the simulation ends */
	pthread_join(monitor, NULL);
	
	/* Wait for all philosopher threads to finish
	** They should exit their loops once is_simulation_over() returns true */
	if (!join_threads(data))
		printf("Error: Thread joining failed\n");
	
	return (1);
}

/*
** main
** ----
** Entry point of the program. Coordinates the entire simulation lifecycle.
** 
** Flow:
** 1. Initializes simulation (parse args + setup data structures)
** 2. Runs simulation (creates threads, monitors, waits for completion)
** 3. Cleans up resources (destroys mutexes, frees memory)
** 
** Parameters:
**   - argc: argument count
**   - argv: argument values
**     Expected: ./philo nb_philos time_to_die time_to_eat time_to_sleep [must_eat_count]
** 
** Returns:
**   - 0 on successful execution
**   - 1 on initialization failure
** 
** The cleanup always happens at the end regardless of simulation outcome
** to ensure no resource leaks (mutexes and memory).
*/
int	main(int argc, char **argv)
{
	t_data	data;

	/* Initialize the simulation (parse arguments and set up data structures) */
	if (!initialize_simulation(argc, argv, &data))
		return (1);
	
	/* Run the simulation (create threads, monitor, and wait for completion) */
	run_simulation(&data);
	
	/* Clean up all resources (destroy mutexes and free memory) */
	cleanup_mutexes(&data);
	cleanup_data(&data);
	
	return (0);
}
