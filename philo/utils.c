/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:13:56 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 18:55:25 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Get current time in milliseconds
** Uses gettimeofday() to get precise timestamp
** Returns time in milliseconds since epoch
*/
long	get_time(void)
{
	struct	timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
	{
		printf("Error: gettimeofday failed\n");
		return (0);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
** More precise sleep than usleep
** usleep() can overshoot by a lot, especially with small values
** This function actively checks time to be more accurate
** milliseconds: time to sleep in milliseconds
*/
void	ft_usleep(long milliseconds)
{
	long	start;
	long	current;

	start = get_time();
	while (1)
	{
		current = get_time();
		if ((current - start) >= milliseconds)
			break ;
		usleep(100);
	}
}

/*
** Print philosopher status with timestamp and mutex protection
** Format: "timestamp_in_ms X status"
** philo: pointer to philosopher
** status: status message to print
*/
void	print_status(t_philo *philo, char *status)
{
	long	timestamp;

	pthread_mutex_lock(&philo->data->print_mutex);
	if (!is_simulation_over(philo->data))
	{
		timestamp = get_time() - philo->data->start_time;
		printf("%ld %d %s\n", timestamp, philo->id, status);
	}
	pthread_mutex_unlock(&philo->data->print_mutex);
}