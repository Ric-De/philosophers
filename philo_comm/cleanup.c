/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:34:52 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:56:54 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** cleanup_mutexes
** ---------------
** Destroys all mutexes to free system resources.
** MUST be called before freeing memory to avoid resource leaks.
** 
** Why destroy mutexes?
**   - Mutexes are system resources (not just memory)
**   - Each mutex holds kernel resources
**   - pthread_mutex_destroy() releases these resources
**   - Without destruction: resource leak (system might run out of mutexes)
** 
** What mutexes are destroyed?
** 1. Fork mutexes (one per philosopher)
** 2. Meal mutexes (one per philosopher)
** 3. Global print mutex (one)
** 4. Global death mutex (one)
** 
** Order doesn't matter for destruction:
**   - Unlike locking, destruction order doesn't cause deadlock
**   - All threads have already exited when cleanup is called
**   - No one is using the mutexes anymore
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Safety checks:
**   - Checks if forks pointer is not NULL before iterating
**   - Checks if philos pointer is not NULL before iterating
**   - Prevents crashes if initialization partially failed
** 
** Called after:
**   - All threads have been joined
**   - No thread is using any mutex anymore
**   - Simulation is completely finished
*/
void	cleanup_mutexes(t_data *data)
{
	int	i;

	/* Destroy all fork mutexes (if they were allocated) */
	if (data->forks)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			/* Destroy each fork mutex
			** Releases system resources associated with the mutex */
			pthread_mutex_destroy(&data->forks[i]);
			i++;
		}
	}
	
	/* Destroy all philosopher meal mutexes (if they were allocated) */
	if (data->philos)
	{
		i = 0;
		while (i < data->nb_philos)
		{
			/* Destroy each philosopher's meal mutex
			** Protects meals_eaten and last_meal_time */
			pthread_mutex_destroy(&data->philos[i].meal_mutex);
			i++;
		}
	}
	
	/* Destroy global print mutex
	** Used for thread-safe console output */
	pthread_mutex_destroy(&data->print_mutex);
	
	/* Destroy global death mutex
	** Used for thread-safe access to someone_died flag */
	pthread_mutex_destroy(&data->death_mutex);
}

/*
** cleanup_data
** ------------
** Frees all dynamically allocated memory to prevent memory leaks.
** Should be called AFTER cleanup_mutexes() to ensure proper order.
** 
** Why free memory?
**   - malloc() allocates memory from heap
**   - Without free(), memory stays allocated even after program ends
**   - Modern OS reclaims memory, but good practice to clean up
**   - Prevents memory leaks (important for long-running or repeated use)
** 
** What is freed?
** 1. Fork array (allocated in init_forks)
** 2. Philosophers array (allocated in init_philosophers)
** 
** Order of operations:
**   1. cleanup_mutexes() - destroy mutexes first
**   2. cleanup_data() - then free memory
** 
** Why this order?
**   - Mutexes must be destroyed before freeing memory containing them
**   - Destroying a mutex after freeing its memory = undefined behavior
**   - Always: destroy resources, then free memory
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Safety:
**   - Checks if pointer is not NULL before freeing
**   - Sets pointer to NULL after freeing (prevents double-free)
**   - Good practice: always NULL-check before free, always NULL-assign after free
** 
** Called at the end of main():
**   - After simulation is complete
**   - After all threads joined
**   - After all mutexes destroyed
**   - Last step before program exit
*/
void	cleanup_data(t_data *data)
{
	/* Free fork array (if it was allocated) */
	if (data->forks)
	{
		free(data->forks);
		data->forks = NULL;  /* Prevent double-free or use-after-free */
	}
	
	/* Free philosopher array (if it was allocated) */
	if (data->philos)
	{
		free(data->philos);
		data->philos = NULL;  /* Prevent double-free or use-after-free */
	}
}
