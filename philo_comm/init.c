/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:13:41 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:55:20 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** init_forks
** ----------
** Initializes fork mutexes. Each fork is represented by a mutex.
** In the Dining Philosophers problem, forks are shared resources between
** adjacent philosophers.
** 
** Structure:
**   - Number of forks = number of philosophers
**   - Fork i is shared between philosopher i and philosopher (i+1) % nb_philos
**   - Each fork is a mutex that must be locked before use
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 on success (all fork mutexes initialized)
**   - 0 on failure (allocation or mutex initialization failed)
** 
** Why mutexes for forks?
**   - Mutexes ensure only one philosopher can hold a fork at a time
**   - Prevents race conditions when multiple philosophers try to grab same fork
**   - Provides exclusive access to shared resources
*/
int	init_forks(t_data *data)
{
	int	i;

	/* Allocate memory for fork mutexes (one per philosopher) */
	data->forks = malloc(sizeof(pthread_mutex_t) * data->nb_philos);
	if (!data->forks)
	{
		printf("Error: Failed to allocate memory for the damn forks\n");
		return (0);
	}
	
	/* Initialize each fork mutex */
	i = 0;
	while (i < data->nb_philos)
	{
		/* Initialize mutex with default attributes (NULL) */
		if (pthread_mutex_init(&data->forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize fork mutex %d\n", i);
			return (0);
		}
		i++;
	}
	
	return (1);
}

/*
** init_mutexes
** ------------
** Initializes the global mutexes used for shared operations across all threads.
** 
** Two global mutexes:
** 1. print_mutex: Protects console output
**    - Prevents multiple threads from printing simultaneously
**    - Ensures clean, non-interleaved output messages
**    - Used when printing philosopher actions and death messages
** 
** 2. death_mutex: Protects the 'someone_died' flag
**    - Ensures thread-safe checking/setting of simulation end condition
**    - Prevents race conditions when checking if simulation should stop
**    - Critical for coordinating thread termination
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 on success (both mutexes initialized)
**   - 0 on failure (mutex initialization failed)
** 
** On failure:
**   - If print_mutex fails, returns immediately
**   - If death_mutex fails, cleans up print_mutex before returning
*/
int	init_mutexes(t_data *data)
{
	/* Initialize print mutex for thread-safe console output */
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize print mutex\n");
		return (0);
	}
	
	/* Initialize death mutex for thread-safe death flag checking */
	if (pthread_mutex_init(&data->death_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize death mutex\n");
		/* Clean up the successfully initialized print_mutex */
		pthread_mutex_destroy(&data->print_mutex);
		return (0);
	}
	
	return (1);
}

/*
** setup_philosopher
** -----------------
** Configures a single philosopher's properties and fork assignments.
** 
** Fork assignment strategy:
**   - Philosopher i has left_fork = fork[i]
**   - Philosopher i has right_fork = fork[(i+1) % nb_philos]
**   - The modulo ensures the last philosopher wraps around to fork[0]
** 
** Example with 5 philosophers:
**   Philo 0: left=fork[0], right=fork[1]
**   Philo 1: left=fork[1], right=fork[2]
**   Philo 2: left=fork[2], right=fork[3]
**   Philo 3: left=fork[3], right=fork[4]
**   Philo 4: left=fork[4], right=fork[0]  <- wraps around
** 
** Parameters:
**   - data: pointer to main data structure
**   - i: index of philosopher to set up (0-based)
** 
** Sets up:
**   - id: philosopher ID (1-based for display purposes)
**   - meals_eaten: initialized to 0
**   - last_meal_time: initialized to 0 (will be set to start_time before threads start)
**   - left_fork: pointer to left fork mutex
**   - right_fork: pointer to right fork mutex
**   - data: pointer back to shared data structure
*/
void	setup_philosopher(t_data *data, int i)
{
	/* Set philosopher ID (1-based for human-readable output) */
	data->philos[i].id = i + 1;
	
	/* Initialize meal counter */
	data->philos[i].meals_eaten = 0;
	
	/* Initialize last meal time (will be updated before simulation starts) */
	data->philos[i].last_meal_time = 0;
	
	/* Assign left fork (fork with same index as philosopher) */
	data->philos[i].left_fork = &data->forks[i];
	
	/* Assign right fork (next fork in circular arrangement)
	** Modulo ensures last philosopher gets fork[0] */
	data->philos[i].right_fork = &data->forks[(i + 1) % data->nb_philos];
	
	/* Link back to shared data structure */
	data->philos[i].data = data;
	
	/* Debug output showing fork assignments */
	printf("(left_fork=%d, right_fork=%d)\n", i,
		(i + 1) % data->nb_philos);
}

/*
** init_philosophers
** -----------------
** Initializes all philosopher structures and their individual meal mutexes.
** 
** Each philosopher has:
**   - A unique ID
**   - A meal counter (tracks how many times they've eaten)
**   - A last meal timestamp (for death detection)
**   - Pointers to their left and right forks
**   - A meal_mutex to protect access to meals_eaten and last_meal_time
**   - A pointer back to shared data
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 on success (all philosophers initialized)
**   - 0 on failure (memory allocation or mutex initialization failed)
** 
** The meal_mutex is crucial because:
**   - Multiple threads (monitor + philosopher) need to access meal info
**   - Prevents race conditions when reading/writing meal data
**   - Monitor thread reads last_meal_time to check for deaths
**   - Philosopher thread updates last_meal_time when eating
*/
int	init_philosophers(t_data *data)
{
	int	i;

	/* Allocate memory for all philosopher structures */
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
	{
		printf("Error: Failed to allocate memory for philosophers\n");
		return (0);
	}
	
	/* Initialize each philosopher */
	i = 0;
	while (i < data->nb_philos)
	{
		/* Initialize the meal mutex for this philosopher
		** This protects access to meals_eaten and last_meal_time */
		if (pthread_mutex_init(&data->philos[i].meal_mutex, NULL) != 0)
		{
			printf("Error: Failed to init meal mutex for philo %d\n", i + 1);
			return (0);
		}
		
		/* Set up philosopher's properties and fork pointers */
		setup_philosopher(data, i);
		i++;
	}
	
	return (1);
}

/*
** init_data
** ---------
** Master initialization function that sets up all data structures needed
** for the simulation.
** 
** Initialization order:
** 1. Initialize flags and basic fields (someone_died, start_time)
** 2. Initialize global mutexes (print_mutex, death_mutex)
** 3. Initialize fork mutexes (one per philosopher)
** 4. Initialize philosopher structures (with meal mutexes and fork assignments)
** 
** Parameters:
**   - data: pointer to main data structure (already has parsed arguments)
** 
** Returns:
**   - 1 on success (everything initialized correctly)
**   - 0 on failure (any sub-initialization failed)
** 
** Initial values:
**   - someone_died: 0 (no one has died yet)
**   - start_time: 0 (will be set when simulation actually starts)
**   - philos: NULL (will be allocated by init_philosophers)
**   - forks: NULL (will be allocated by init_forks)
** 
** On failure, caller is responsible for cleanup to avoid resource leaks.
*/
int	init_data(t_data *data)
{
	/* Initialize simulation state flags */
	data->someone_died = 0;  /* No deaths yet */
	data->start_time = 0;    /* Will be set when simulation starts */
	
	/* Initialize pointers to NULL (will be allocated by init functions) */
	data->philos = NULL;
	data->forks = NULL;
	
	/* Initialize global mutexes (print and death) */
	if (!init_mutexes(data))
		return (0);
	
	/* Initialize fork mutexes (one per philosopher) */
	if (!init_forks(data))
	{
		return (0);
	}
	
	/* Initialize all philosopher structures */
	if (!init_philosophers(data))
	{
		return (0);
	}
	
	return (1);
}
