/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:05:55 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 13:43:05 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_time_value(void)
{
	struct timeval	timestamp;

	gettimeofday(&timestamp, NULL);
	return ((timestamp.tv_sec * 1000) + (timestamp.tv_usec / 1000));
}

time_t	get_mstime(void)
{
	static time_t	start_time = 0;
	int				r;

	if (start_time == 0)
		start_time = get_time_value();
	r = get_time_value() - start_time;
	return (r);
}

void	set_state(t_philo *p, char *state)
{
	pthread_mutex_lock(p->info.starvation);
	pthread_mutex_lock(p->info.time);
	pthread_mutex_lock(p->info.printor);
	if (*p->info.end != 1)
		printf("%li The philosopher 🗿 [%d] is %s\n", get_mstime(), p->id,
			state);
	pthread_mutex_unlock(p->info.printor);
	pthread_mutex_unlock(p->info.time);
	pthread_mutex_unlock(p->info.starvation);
}

void	ft_usleep(int ms)
{
	long int	time;

	time = get_time_value();
	while (get_time_value() - time < ms)
		usleep(100);
}
