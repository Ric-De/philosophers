/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 02:41:30 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:56:18 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** select_fork_order
** -----------------
** Implements the GLOBAL ORDERING strategy for deadlock prevention.
** This is the heart of the deadlock prevention algorithm!
** 
** THE PROBLEM: Dining Philosophers Deadlock
** ------------------------------------------
** In a circular arrangement, if all philosophers grab their left fork
** simultaneously, all will wait forever for their right fork -> DEADLOCK!
** 
** Example with 5 philosophers (without ordering):
**   Philo 1 grabs fork 1, waits for fork 2
**   Philo 2 grabs fork 2, waits for fork 3
**   Philo 3 grabs fork 3, waits for fork 4
**   Philo 4 grabs fork 4, waits for fork 5
**   Philo 5 grabs fork 5, waits for fork 1
**   -> CIRCULAR WAIT! Everyone stuck forever!
** 
** THE SOLUTION: Global Ordering
** ------------------------------
** Break the circular wait by making ONE philosopher take forks in
** reverse order.
** 
** Strategy:
**   - Most philosophers (1 to N-1): take LEFT fork first, then RIGHT
**   - Last philosopher (N): take RIGHT fork first, then LEFT
** 
** Why does this work?
**   - The last philosopher competes for DIFFERENT forks initially
**   - This breaks the circular dependency chain
**   - At least one philosopher can always make progress
**   - No circular wait = No deadlock!
** 
** Example with 5 philosophers (with ordering):
**   Philo 1: left=fork[1], right=fork[2]  -> takes 1 first
**   Philo 2: left=fork[2], right=fork[3]  -> takes 2 first
**   Philo 3: left=fork[3], right=fork[4]  -> takes 3 first
**   Philo 4: left=fork[4], right=fork[5]  -> takes 4 first
**   Philo 5: left=fork[5], right=fork[1]  -> takes 1 first (REVERSED!)
**                                             (NOT 5, breaking the cycle)
** 
** Now Philo 5 competes with Philo 1 for fork[1] initially,
** not creating a circular wait.
** 
** Parameters:
**   - philo: pointer to philosopher
**   - first: output pointer to first fork to acquire
**   - second: output pointer to second fork to acquire
** 
** Sets:
**   - *first: pointer to the fork to lock first
**   - *second: pointer to the fork to lock second
*/
void	select_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	/* Check if this is the LAST philosopher (ID == total number)
	** IDs are 1-based, so last philosopher has ID = nb_philos */
	if (philo->id == philo->data->nb_philos)
	{
		/* LAST PHILOSOPHER: Reverse the order!
		** Take RIGHT fork first, then LEFT fork
		** This breaks the circular wait condition */
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		/* NORMAL PHILOSOPHERS: Regular order
		** Take LEFT fork first, then RIGHT fork
		** This is the standard pattern for most philosophers */
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

/*
** lock_first_fork
** ---------------
** Safely locks the first fork with simulation end checking.
** 
** Why is this a separate function?
**   - Need to check if simulation ended BETWEEN fork acquisitions
**   - If simulation ended after locking first fork, must unlock it
**   - Prevents holding a mutex when thread is about to exit
**   - Clean exit prevents mutex deadlocks and resource leaks
** 
** Flow:
** 1. Lock the first fork (blocks if fork is being used)
** 2. Print "has taken a fork" status
** 3. Check if simulation ended while we were waiting
** 4. If ended: unlock the fork and return failure
** 5. If not ended: keep fork locked and return success
** 
** Parameters:
**   - philo: pointer to philosopher taking fork
**   - first_fork: pointer to the mutex representing first fork to take
** 
** Returns:
**   - 1 if fork successfully locked and simulation still running
**   - 0 if simulation ended (fork was unlocked before returning)
** 
** Thread safety:
**   - pthread_mutex_lock() blocks until mutex available
**   - Only one thread can hold a mutex at a time
**   - is_simulation_over() is thread-safe (uses death_mutex)
** 
** Why check simulation end here?
**   - A death could have been detected while waiting for fork
**   - Need to exit cleanly without holding resources
**   - Prevents zombie mutexes (locked but never unlocked)
*/
int	lock_first_fork(t_philo *philo, pthread_mutex_t *first_fork)
{
	/* Lock the first fork (blocks if another philosopher has it)
	** This is where we might wait if the fork is busy */
	pthread_mutex_lock(first_fork);
	
	/* Announce that we took a fork */
	print_status(philo, "has taken a fork");
	
	/* Check if simulation ended while we were waiting for the fork
	** Death could have been detected during the wait */
	if (is_simulation_over(philo->data))
	{
		/* Simulation ended - clean up and return failure
		** MUST unlock the fork we just locked to prevent deadlock */
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	
	/* Success - fork is locked and simulation is still running */
	return (1);
}
