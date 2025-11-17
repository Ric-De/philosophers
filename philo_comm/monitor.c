/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:27:31 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:56:36 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** is_simulation_over
** ------------------
** Thread-safe check for simulation termination flag.
** Used by ALL threads to determine if they should continue running.
** 
** Why is this function needed?
**   - someone_died flag is accessed by multiple threads simultaneously
**   - Without mutex protection, race conditions could occur
**   - One thread might read while another writes -> undefined behavior
**   - Mutex ensures atomic read of the flag
** 
** Protected by death_mutex:
**   - Only one thread can read/write someone_died at a time
**   - Prevents race conditions
**   - Ensures consistent state across all threads
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 if someone died or all philosophers finished eating (simulation should end)
**   - 0 if simulation should continue
** 
** Called by:
**   - All philosopher threads (to check if they should continue their routine)
**   - print_status() (to decide whether to print)
**   - take_forks() (to check between fork acquisitions)
**   - Various action functions
** 
** Why make it a function instead of direct access?
**   - Encapsulates mutex locking/unlocking
**   - Prevents forgetting to lock/unlock
**   - Single point of control for flag access
**   - Easier to maintain and understand
*/
int	is_simulation_over(t_data *data)
{
	int	result;

	/* Lock death_mutex to safely read someone_died flag */
	pthread_mutex_lock(&data->death_mutex);
	
	/* Read the flag value */
	result = data->someone_died;
	
	/* Unlock death_mutex so other threads can check */
	pthread_mutex_unlock(&data->death_mutex);
	
	return (result);
}

/*
** check_philosopher_death
** -----------------------
** Checks if a specific philosopher has died from starvation.
** This is where death detection happens!
** 
** DEATH CONDITION:
**   A philosopher dies if: (current_time - last_meal_time) > time_to_die
** 
** Example:
**   time_to_die = 400ms
**   last_meal_time = 100ms
**   current_time = 550ms
**   time_since_meal = 550 - 100 = 450ms
**   450ms > 400ms -> PHILOSOPHER DIED!
** 
** Flow:
** 1. Get current time
** 2. Safely read philosopher's last_meal_time (protected by meal_mutex)
** 3. Calculate time since last meal
** 4. If time exceeds time_to_die:
**    a. Print death message (with timestamp)
**    b. Set someone_died flag to 1
**    c. Return 1 (death detected)
** 5. Otherwise return 0 (still alive)
** 
** Parameters:
**   - data: pointer to main data structure
**   - i: index of philosopher to check (0-based)
** 
** Returns:
**   - 1 if philosopher died
**   - 0 if philosopher still alive
** 
** Thread safety:
**   - last_meal_time read is protected by philosopher's meal_mutex
**   - Death message printing is protected by print_mutex
**   - someone_died flag write is protected by death_mutex
** 
** Why multiple mutexes?
**   - meal_mutex: protects philosopher's personal data (meals_eaten, last_meal_time)
**   - print_mutex: prevents interleaved console output
**   - death_mutex: protects global simulation state (someone_died)
*/
int	check_philosopher_death(t_data *data, int i)
{
	long	current_time;
	long	time_since_meal;
	long	last_meal;

	/* Get current timestamp */
	current_time = get_time();
	
	/* Safely read last_meal_time
	** Must be protected because philosopher thread updates it when eating */
	pthread_mutex_lock(&data->philos[i].meal_mutex);
	last_meal = data->philos[i].last_meal_time;
	pthread_mutex_unlock(&data->philos[i].meal_mutex);
	
	/* Calculate how long since philosopher last ate */
	time_since_meal = current_time - last_meal;
	
	/* Check if philosopher starved (time_since_meal > time_to_die) */
	if (time_since_meal > data->time_to_die)
	{
		/* DEATH DETECTED!
		** Print death message (thread-safe with print_mutex) */
		pthread_mutex_lock(&data->print_mutex);
		printf("%ld %d died\n", current_time - data->start_time,
			data->philos[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		
		/* Set someone_died flag to end simulation
		** Protected by death_mutex for thread safety */
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		
		/* Return 1 to indicate death was detected */
		return (1);
	}
	
	/* Philosopher is still alive */
	return (0);
}

/*
** check_death
** -----------
** Checks if ANY philosopher has died.
** Iterates through all philosophers and checks each one.
** 
** Flow:
**   - Loop through all philosophers
**   - Check each one for death using check_philosopher_death()
**   - If any death detected, return immediately
**   - If all alive, return 0
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 if any philosopher died
**   - 0 if all philosophers still alive
** 
** Early exit:
**   - Returns immediately when first death is found
**   - No need to check remaining philosophers
**   - Simulation is over once anyone dies
*/
int	check_death(t_data *data)
{
	int	i;

	i = 0;
	/* Check each philosopher for death */
	while (i < data->nb_philos)
	{
		/* If this philosopher died, return immediately */
		if (check_philosopher_death(data, i))
			return (1);
		i++;
	}
	
	/* All philosophers still alive */
	return (0);
}

/*
** check_all_ate
** --------------
** Checks if all philosophers have eaten the required number of times.
** Only relevant when must_eat_count is specified (not -1).
** 
** Purpose:
**   - Simulation can end successfully if everyone ate enough
**   - Alternative ending to death
**   - Used when user specifies: ./philo 5 800 200 200 7
**     (each philosopher must eat 7 times)
** 
** Flow:
** 1. If must_eat_count is -1 (unlimited), return 0 immediately
** 2. Check each philosopher's meals_eaten counter
** 3. If ALL philosophers ate >= must_eat_count:
**    - Set someone_died flag to 1 (ends simulation)
**    - Return 1 (success condition met)
** 4. If ANY philosopher hasn't eaten enough:
**    - Return 0 (simulation continues)
** 
** Parameters:
**   - data: pointer to main data structure
** 
** Returns:
**   - 1 if all philosophers ate enough (simulation should end)
**   - 0 if not all philosophers finished eating (simulation continues)
** 
** Thread safety:
**   - Each philosopher's meals_eaten is protected by their meal_mutex
**   - someone_died flag write is protected by death_mutex
** 
** Early exit optimization:
**   - If we find ONE philosopher who hasn't eaten enough, stop checking
**   - No need to check remaining philosophers
**   - Simulation must continue until ALL have eaten enough
** 
** Note: Reuses someone_died flag to end simulation
**   - Flag name is misleading (no one died in this case)
**   - But effect is same: ends simulation
**   - Could be renamed to "simulation_ended" for clarity
*/
int	check_all_ate(t_data *data)
{
	int	i;
	int	all_ate_enough;

	/* If no meal requirement specified, return 0 (don't end simulation) */
	if (data->must_eat_count == -1)
		return (0);
	
	/* Assume all ate enough until we find one who hasn't */
	all_ate_enough = 1;
	
	i = 0;
	/* Check each philosopher's meal count */
	while (i < data->nb_philos)
	{
		/* Safely read meals_eaten (protected by meal_mutex) */
		pthread_mutex_lock(&data->philos[i].meal_mutex);
		if (data->philos[i].meals_eaten < data->must_eat_count)
			all_ate_enough = 0;  /* Found someone who hasn't eaten enough */
		pthread_mutex_unlock(&data->philos[i].meal_mutex);
		
		/* Early exit: if we found someone who hasn't eaten enough, stop checking */
		if (!all_ate_enough)
			break ;
		
		i++;
	}
	
	/* If all philosophers ate enough, end simulation successfully */
	if (all_ate_enough)
	{
		/* Set someone_died flag to end simulation
		** (Yes, flag name is confusing - no one died, but simulation ends) */
		pthread_mutex_lock(&data->death_mutex);
		data->someone_died = 1;
		pthread_mutex_unlock(&data->death_mutex);
		
		return (1);  /* Success! Everyone ate enough */
	}
	
	/* Not everyone has eaten enough yet, simulation continues */
	return (0);
}

/*
** monitor_routine
** ---------------
** The monitor thread's main function. Watches over all philosophers.
** This thread determines when the simulation should end.
** 
** The monitor's job:
**   - Continuously check if any philosopher died
**   - Continuously check if all philosophers ate enough
**   - End simulation when either condition is met
** 
** Termination conditions:
**   1. ANY philosopher dies from starvation
**   2. ALL philosophers ate required number of meals (if specified)
** 
** Flow:
** 1. Enter infinite loop
** 2. Check for deaths (check_death)
** 3. If death detected, break loop
** 4. Check if everyone ate enough (check_all_ate)
** 5. If everyone finished, break loop
** 6. Sleep briefly (1ms) to avoid hogging CPU
** 7. Repeat until simulation ends
** 
** Parameters:
**   - arg: void pointer cast to (t_data *) - shared data structure
** 
** Returns:
**   - NULL when monitor exits (standard pthread return)
** 
** Why separate monitor thread?
**   - Death detection needs to happen independently of philosophers
**   - Can't rely on philosophers to detect their own deaths
**   - Centralized monitoring is cleaner and more reliable
**   - Philosopher threads focus on their routine, monitor focuses on checking
** 
** Sleep duration (1000 microseconds = 1ms):
**   - Prevents CPU hogging (busy waiting would waste resources)
**   - 1ms is frequent enough to catch deaths quickly
**   - 1ms is brief enough not to delay death detection significantly
**   - Balance between responsiveness and CPU efficiency
** 
** After monitor exits:
**   - Main thread joins monitor with pthread_join()
**   - Main thread then joins all philosopher threads
**   - Cleanup happens (mutexes destroyed, memory freed)
**   - Program exits cleanly
*/
void	*monitor_routine(void *arg)
{
	t_data	*data;

	/* Cast void pointer to t_data pointer */
	data = (t_data *)arg;
	
	/* Monitoring loop - runs until simulation ends */
	while (1)
	{
		/* Check if any philosopher died from starvation */
		if (check_death(data))
		{
			/* Death detected - end monitoring loop */
			break ;
		}
		
		/* Check if all philosophers ate enough (if meal limit specified) */
		if (check_all_ate(data))
			break ;  /* Everyone finished - end monitoring loop */
		
		/* Brief sleep to prevent CPU hogging
		** 1000 microseconds = 1 millisecond
		** Frequent enough to catch deaths quickly
		** Brief enough not to waste CPU resources */
		usleep(1000);
	}
	
	/* Monitor exits - simulation is over
	** Philosopher threads will detect this via is_simulation_over() */
	return (NULL);
}
