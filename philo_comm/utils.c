/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:13:56 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:55:33 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** get_time
** --------
** Retrieves the current time in milliseconds since the Unix epoch (Jan 1, 1970).
** Used throughout the simulation for timestamps and time calculations.
** 
** How it works:
** 1. Calls gettimeofday() to get current time with microsecond precision
** 2. Converts to milliseconds: (seconds * 1000) + (microseconds / 1000)
** 
** Returns:
**   - Current time in milliseconds on success
**   - 0 on failure (gettimeofday error)
** 
** Used for:
**   - Recording simulation start time
**   - Calculating time elapsed since simulation start
**   - Recording when each philosopher last ate
**   - Checking if time_to_die has been exceeded
**   - Precise timing in ft_usleep
** 
** Why milliseconds?
**   - All time parameters in the problem are in milliseconds
**   - Millisecond precision is sufficient for this simulation
**   - Easier to work with than microseconds
** 
** Example:
**   start_time = get_time();              // Record start
**   current_time = get_time();            // Get current time
**   elapsed = current_time - start_time;  // Calculate elapsed time
*/
long	get_time(void)
{
	struct timeval	tv;

	/* Get current time with microsecond precision
	** tv.tv_sec = seconds since epoch
	** tv.tv_usec = microseconds within current second */
	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		return (0);
	}
	
	/* Convert to milliseconds:
	** - tv_sec * 1000: convert seconds to milliseconds
	** - tv_usec / 1000: convert microseconds to milliseconds
	** - Add them together for total milliseconds */
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** ft_usleep
** ---------
** Custom sleep function that provides more precise timing than standard usleep().
** 
** Why not use standard usleep()?
**   - usleep() can overshoot significantly, especially with small values
**   - It may sleep longer than requested due to system scheduling
**   - For time-critical simulations, this imprecision is problematic
** 
** How this works:
** 1. Records start time
** 2. Continuously checks current time in a loop
** 3. Exits when desired milliseconds have elapsed
** 4. Uses short usleep(100) calls to prevent CPU hogging
** 
** Parameters:
**   - milliseconds: time to sleep in milliseconds
** 
** Trade-offs:
**   - More accurate than usleep() for small values
**   - Uses more CPU due to active polling
**   - usleep(100) between checks reduces CPU usage while maintaining precision
** 
** Example usage:
**   ft_usleep(200);  // Sleep for exactly 200ms (as close as possible)
*/
void	ft_usleep(long milliseconds)
{
	long	start;
	long	current;

	/* Record the start time */
	start = get_time();
	
	/* Loop until desired time has elapsed */
	while (1)
	{
		/* Get current time */
		current = get_time();
		
		/* Check if we've slept long enough */
		if ((current - start) >= milliseconds)
			break ;
		
		/* Small sleep to prevent CPU hogging
		** 100 microseconds is short enough to maintain precision
		** but long enough to reduce CPU usage */
		usleep(100);
	}
}

/*
** print_status
** ------------
** Thread-safe printing of philosopher status messages.
** All output in the simulation goes through this function.
** 
** Format: "timestamp_in_ms philosopher_id status"
** Example: "150 3 is eating"
** 
** Thread safety:
**   - Uses print_mutex to ensure only one thread prints at a time
**   - Prevents interleaved or garbled output
**   - Critical in multithreaded environment
** 
** Simulation end check:
**   - Only prints if simulation is still running
**   - Prevents printing after a death has been detected
**   - Uses is_simulation_over() to check death flag
** 
** Parameters:
**   - philo: pointer to the philosopher performing the action
**   - status: string describing the action (e.g., "is eating", "is sleeping")
** 
** Possible status messages:
**   - "has taken a fork"
**   - "is eating"
**   - "is sleeping"
**   - "is thinking"
**   - "died"
** 
** Why check is_simulation_over()?
**   - Death message should be the last thing printed
**   - Prevents philosophers from printing after someone died
**   - Keeps output clean and relevant
** 
** Example output sequence:
**   0 1 has taken a fork
**   0 1 has taken a fork
**   0 1 is eating
**   210 1 is sleeping
**   410 1 is thinking
**   [...]
**   820 3 died
*/
void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	/* Acquire print mutex for thread-safe output */
	pthread_mutex_lock(&philo->data->print_mutex);
	
	/* Only print if simulation hasn't ended
	** This prevents printing after a death has been detected */
	if (!is_simulation_over(philo->data))
	{
		/* Calculate timestamp as milliseconds since simulation start */
		timestamp = get_time() - philo->data->start_time;
		
		/* Print in the required format: timestamp philosopher_id status */
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	
	/* Release print mutex so other threads can print */
	pthread_mutex_unlock(&philo->data->print_mutex);
}
