/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:51:46 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:55:58 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** take_forks
** ----------
** Philosopher attempts to acquire both forks (mutexes) needed for eating.
** This is THE critical function for deadlock prevention.
** 
** DEADLOCK PREVENTION: Global Ordering Strategy
** ----------------------------------------------
** The key to preventing deadlock is the ORDER in which forks are acquired.
** 
** Strategy:
**   - Most philosophers take left fork first, then right fork
**   - LAST philosopher takes RIGHT fork first, then left fork
**   - This breaks the circular wait condition that causes deadlock
** 
** Why does this work?
**   Without global ordering: All philosophers could grab their left fork
**   simultaneously, then all wait for their right fork -> DEADLOCK!
** 
**   With global ordering: Last philosopher taking right fork first means
**   they're competing for a different fork initially, breaking the cycle.
** 
** Special case: Single philosopher
**   - Only one philosopher, only one fork
**   - Takes the fork and waits forever (can't eat with one fork)
**   - Simulation will detect death from starvation
** 
** Parameters:
**   - philo: pointer to philosopher attempting to take forks
** 
** Returns:
**   - 1 if both forks successfully acquired (ready to eat)
**   - 0 if simulation ended while trying to take forks
** 
** Flow:
** 1. Check for single philosopher edge case
** 2. Determine fork order using global ordering strategy
** 3. Lock first fork (with simulation end check)
** 4. Lock second fork
** 5. Return success
*/
int	take_forks(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	/* EDGE CASE: Single philosopher
	** With only one philosopher and one fork, they can't eat
	** (need two forks to eat). Take the fork, print status,
	** then wait until simulation ends (death from starvation) */
	if (philo->data->nb_philos == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_status(philo, "has taken a fork");
		
		/* Wait until monitor detects death (can't eat with one fork) */
		while (!is_simulation_over(philo->data))
			usleep(100);
		
		pthread_mutex_unlock(philo->left_fork);
		return (0);
	}
	
	/* DEADLOCK PREVENTION: Determine fork acquisition order
	** Uses global ordering strategy implemented in select_fork_order() */
	select_fork_order(philo, &first_fork, &second_fork);
	
	/* Lock first fork (based on global ordering)
	** Checks if simulation ended while waiting */
	if (!lock_first_fork(philo, first_fork))
		return (0);
	
	/* Lock second fork
	** No need to check simulation end here because we'll check
	** right after in philo_eat() before actually eating */
	pthread_mutex_lock(second_fork);
	print_status(philo, "has taken a fork");
	
	/* Successfully acquired both forks */
	return (1);
}

/*
** drop_forks
** ----------
** Releases both fork mutexes after eating.
** 
** Simple operation:
**   - Unlocks left fork
**   - Unlocks right fork
** 
** Order doesn't matter for unlocking:
**   - Unlike locking, unlock order doesn't affect deadlock
**   - Both forks simply released back to available pool
**   - Other philosophers can now acquire them
** 
** Parameters:
**   - philo: pointer to philosopher dropping forks
** 
** Why separate from philo_eat()?
**   - Cleaner code organization
**   - Easier to understand flow
**   - Consistent with take_forks() structure
*/
void	drop_forks(t_philo *philo)
{
	/* Release left fork mutex */
	pthread_mutex_unlock(philo->left_fork);
	
	/* Release right fork mutex */
	pthread_mutex_unlock(philo->right_fork);
}

/*
** philo_eat
** ---------
** Philosopher eating routine. This is where the magic happens!
** 
** CRITICAL SEQUENCE (must be in this exact order):
** 1. Take forks (acquire mutexes)
** 2. Update last_meal_time (VITAL for death detection!)
** 3. Print eating status
** 4. Sleep for time_to_eat (simulate eating)
** 5. Increment meals_eaten counter
** 6. Drop forks (release mutexes)
** 
** Why is last_meal_time so important?
**   - Monitor thread checks: (current_time - last_meal_time) > time_to_die
**   - If we don't update it, philosopher will be detected as dead
**   - MUST be updated IMMEDIATELY after acquiring forks
**   - MUST be protected by meal_mutex (monitor reads it too)
** 
** Thread safety:
**   - last_meal_time: protected by meal_mutex
**   - meals_eaten: protected by meal_mutex
**   - Prevents race conditions with monitor thread
** 
** Parameters:
**   - philo: pointer to philosopher eating
** 
** Flow:
** 1. Try to acquire forks
**    - If fail (single philo or simulation ended), return early
** 2. Lock meal_mutex
** 3. Update last_meal_time to current time (CRITICAL!)
** 4. Unlock meal_mutex
** 5. Print "is eating" status
** 6. Sleep for time_to_eat milliseconds (simulate eating)
** 7. Lock meal_mutex
** 8. Increment meals_eaten counter
** 9. Unlock meal_mutex
** 10. Drop forks (let other philosophers use them)
*/
void	philo_eat(t_philo *philo)
{
	int	got_forks;

	/* Try to acquire both forks using global ordering strategy */
	got_forks = take_forks(philo);
	
	/* If we couldn't get forks (simulation ended or single philo), return */
	if (!got_forks)
		return ;
	
	/* CRITICAL SECTION: Update last meal time
	** This MUST happen immediately after getting forks
	** Monitor thread reads this to check for death
	** Protected by meal_mutex to prevent race conditions */
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	pthread_mutex_unlock(&philo->meal_mutex);
	
	/* Announce eating status */
	print_status(philo, "is eating");
	
	/* Simulate eating: sleep for time_to_eat milliseconds
	** During this time, philosopher holds both forks
	** Other philosophers waiting for these forks must wait */
	ft_usleep(philo->data->time_to_eat);
	
	/* Update meal counter (used to check if everyone ate enough)
	** Protected by meal_mutex as monitor reads this too */
	pthread_mutex_lock(&philo->meal_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	
	/* Release both forks so other philosophers can use them */
	drop_forks(philo);
}

/*
** philo_sleep
** -----------
** Philosopher sleeping routine. Simple recovery period after eating.
** 
** What happens:
**   - Prints "is sleeping" status
**   - Sleeps for time_to_sleep milliseconds
**   - Does nothing else (just resting)
** 
** Why sleeping?
**   - Simulates digestion time
**   - Gives other philosophers chance to eat
**   - Part of the natural eat-sleep-think cycle
**   - Prevents any philosopher from monopolizing resources
** 
** Parameters:
**   - philo: pointer to philosopher sleeping
** 
** Note: No mutex operations needed here
**   - Not accessing/modifying any shared data during sleep
**   - Just pausing execution for specified time
*/
void	philo_sleep(t_philo *philo)
{
	/* Announce sleeping status */
	print_status(philo, "is sleeping");
	
	/* Sleep for the specified duration
	** During this time, philosopher does nothing
	** Forks were already released in philo_eat() */
	ft_usleep(philo->data->time_to_sleep);
}

/*
** philo_think
** -----------
** Philosopher thinking routine. Helps prevent starvation.
** 
** Purpose:
**   - Provides a buffer between actions
**   - Helps odd-numbered groups avoid starvation
**   - Gives other philosophers opportunity to grab forks
** 
** Thinking time strategy:
**   - ODD number of philosophers: think for 1ms
**     (Helps prevent starvation in odd-numbered circular arrangement)
**   - EVEN number of philosophers: think for 0ms (no delay needed)
**     (Even arrangement naturally prevents starvation)
** 
** Why different for odd vs even?
**   - Odd numbers create natural imbalance in fork access
**   - Small thinking delay helps rebalance access patterns
**   - Even numbers don't have this issue (pairs naturally alternate)
** 
** Parameters:
**   - philo: pointer to philosopher thinking
** 
** Thinking time kept minimal:
**   - Want to maximize eating opportunities
**   - Just enough delay to prevent starvation
**   - Too much thinking = risk of death from not eating enough
*/
void	philo_think(t_philo *philo)
{
	long	think_time;

	/* Announce thinking status */
	print_status(philo, "is thinking");
	
	/* Determine thinking duration based on number of philosophers
	** Odd number: 1ms thinking time (helps prevent starvation)
	** Even number: 0ms thinking time (no delay needed) */
	if (philo->data->nb_philos % 2 != 0)
		think_time = 1;
	else
		think_time = 0;
	
	/* Sleep for thinking duration (if > 0) */
	if (think_time > 0)
		ft_usleep(think_time);
}
