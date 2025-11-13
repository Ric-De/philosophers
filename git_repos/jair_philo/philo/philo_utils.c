/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 17:33:19 by jaacosta          #+#    #+#             */
/*   Updated: 2025/05/08 19:31:03 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

void	ft_putstr_fd(char *s, int fd)
{
	if (!s)
		return ((void) NULL);
	while (*s)
		write(fd, s++, 1);
}

void	clean(t_data *data, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < data->num_philos)
	{
		pthread_mutex_destroy(&data->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&data->print_mutex);
	pthread_mutex_destroy(&data->stop_mutex);
	free(data->forks);
	free(philos);
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(time_t time)
{
	time_t	elapsed;
	time_t	start_time;

	start_time = get_time();
	while (1)
	{
		elapsed = get_time() - start_time;
		if (elapsed >= time)
			break ;
		usleep(500);
	}
	return (1);
}

void	precise_usleep(long long miliseconds, t_data *data)
{
	long long	start;

	start = get_time();
	while (get_time() - start < miliseconds)
	{
		if (check_stop(data))
			break ;
		usleep(1000);
	}
}
