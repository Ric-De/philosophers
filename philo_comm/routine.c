/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:20:27 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:55:46 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** philosopher_routine
** -------------------
** The main lifecycle function that each philosopher thread executes.
** This is the "brain" of each philosopher - their daily routine.
** 
** The eternal cycle (until death or completion):
**   EAT -> SLEEP -> THINK -> (repeat)
** 
** Why this order?
**   - EAT: Philosopher needs food to survive
**   - SLEEP: After eating, philosopher gets tired and must sleep
**   - THINK: After waking up, philosopher contemplates before getting hungry again
** 
** Simulation termination:
**   - Checks is_simulation_over() before and after each action
**   - Stops immediately when someone dies or all philosophers finish eating
**   - Multiple checks ensure quick response to simulation end
** 
** Parameters:
**   - arg: void pointer that must be cast to (t_philo *)
**          Contains all philosopher-specific data
** 
** Returns:
**   - NULL when routine completes (standard pthread return)
** 
** Thread lifecycle:
** 1. Thread created by pthread_create()
** 2. Enters this routine
** 3. Loops through eat/sleep/think cycle
** 4. Exits when simulation ends
** 5. Thread joined by pthread_join() in main
** 
** Race condition prevention:
**   - All shared data access is mutex-protected
**   - is_simulation_over() safely checks death flag
**   - Each action function handles its own synchronization
*/
void	*philosopher_routine(void *arg)
{
	t_philo	*philo;

	/* Cast void pointer to philosopher structure */
	philo = (t_philo *)arg;
	
	/* Main lifecycle loop - continues until simulation ends */
	while (!is_simulation_over(philo->data))
	{
		/* EAT: Try to grab forks and eat
		** This is the critical section where deadlock prevention matters */
		philo_eat(philo);
		
		/* Check if simulation ended during eating */
		if (is_simulation_over(philo->data))
			break ;
		
		/* SLEEP: Rest after eating
		** Philosophers must sleep for time_to_sleep milliseconds */
		philo_sleep(philo);
		
		/* Check if simulation ended during sleeping */
		if (is_simulation_over(philo->data))
			break ;
		
		/* THINK: Contemplate life before getting hungry again
		** Thinking time helps prevent starvation in odd-numbered groups */
		philo_think(philo);
		
		/* Check if simulation ended during thinking */
		if (is_simulation_over(philo->data))
			break ;
	}
	
	/* Return NULL as required by pthread function signature */
	return (NULL);
}

/*
** create_threads
** --------------
** Creates all philosopher threads, starting the simulation.
** Each thread runs philosopher_routine() independently.
** 
** Flow:
** 1. Iterates through all philosophers
** 2. Creates a thread for each philosopher
** 3. Each thread starts executing philosopher_routine()
** 
** Parameters:
**   - data: pointer to main data structure containing all philosophers
** 
** Returns:
**   - 1 on success (all threads created)
**   - 0 on failure (any thread creation failed)
** 
** Thread creation details:
**   - pthread_create() creates a new thread
**   - Thread handle stored in philos[i].thread
**   - NULL attributes (use defaults)
**   - philosopher_routine is the function each thread will execute
**   - &data->philos[i] is passed as argument to the routine
** 
** After this function:
**   - All philosophers are running concurrently
**   - Each is trying to eat/sleep/think
**   - Race conditions prevented by mutexes
**   - Monitor thread will watch for deaths
** 
** Error handling:
**   - If any thread creation fails, returns 0
**   - Caller should handle cleanup of already-created threads
**   - Thread creation rarely fails (usually memory/resource issues)
*/
int	create_threads(t_data *data)
{
	int	i;

	i = 0;
	/* Create a thread for each philosopher */
	while (i < data->nb_philos)
	{
		/* pthread_create parameters:
		** 1. Thread handle: where to store thread ID
		** 2. Thread attributes: NULL = use defaults
		** 3. Start routine: function thread will execute
		** 4. Argument: data passed to start routine */
		if (pthread_create(&data->philos[i].thread, NULL,
				philosopher_routine, &data->philos[i]) != 0)
		{
			printf("Error: Failed to create thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		i++;
	}
	
	return (1);
}

/*
** join_threads
** ------------
** Waits for all philosopher threads to finish execution.
** This ensures proper cleanup and prevents zombie threads.
** 
** What is thread joining?
**   - pthread_join() blocks until specified thread terminates
**   - Allows parent thread to wait for children threads
**   - Cleans up thread resources
**   - Ensures orderly shutdown
** 
** Flow:
** 1. Iterates through all philosophers
** 2. Joins each thread (waits for it to finish)
** 3. Thread resources are cleaned up after join
** 
** Parameters:
**   - data: pointer to main data structure containing all philosophers
** 
** Returns:
**   - 1 on success (all threads joined successfully)
**   - 0 on failure (any thread join failed)
** 
** When are threads ready to join?
**   - After monitor detects simulation end
**   - Philosophers exit their routines when is_simulation_over() returns true
**   - Each thread returns from philosopher_routine()
**   - Then pthread_join() can successfully complete
** 
** Why join threads?
**   - Prevents memory leaks (threads hold resources)
**   - Ensures orderly shutdown
**   - Allows parent to know when all work is complete
**   - Without joining, threads become "zombies"
** 
** Thread join rarely fails:
**   - Main failure case: thread ID is invalid
**   - Or thread was already joined
**   - Or thread is not joinable (detached)
*/
int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	/* Join each philosopher thread */
	while (i < data->nb_philos)
	{
		/* pthread_join parameters:
		** 1. Thread handle: which thread to wait for
		** 2. Return value pointer: NULL = we don't need thread's return value */
		if (pthread_join(data->philos[i].thread, NULL) != 0)
		{
			printf("Error: Failed to join thread for philo %d\n",
				data->philos[i].id);
			return (0);
		}
		i++;
	}
	
	return (1);
}
