/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delfos.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:05:59 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 13:06:00 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	eating(t_philo *p)
{
	p->foods++;
	pthread_mutex_lock(p->info.time);
	p->last_noodle = get_mstime();
	pthread_mutex_unlock(p->info.time);
	set_state(p, STR_EAT);
	ft_usleep(p->info.time2_eat);
	pthread_mutex_unlock(p->left_fork);
	pthread_mutex_unlock(p->right_fork);
	set_state(p, STR_FORK2);
	pthread_mutex_lock(p->info.pleased);
	if (p->info.max_eat != -1 && p->foods >= p->info.max_eat)
	{
		p->state = SATISFIED;
		pthread_mutex_unlock(p->info.pleased);
		return (-1);
	}
	pthread_mutex_unlock(p->info.pleased);
	return (0);
}

static int	zzz(t_philo *p)
{
	if (eating(p) < 0)
		return (-1);
	set_state(p, STR_SLEEP);
	ft_usleep(p->info.time2_sleep);
	return (0);
}

static void	take_forks(t_philo *p, int i)
{
	if (i == 2)
	{
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
		set_state(p, STR_FORK);
	}
	else
	{
		pthread_mutex_lock(p->right_fork);
		pthread_mutex_lock(p->left_fork);
		set_state(p, STR_FORK);
	}
}

void	*the_last_supper(void *arg)
{
	t_philo	*p;

	p = (t_philo *)arg;
	if (p->id % 2 != 0)
		usleep(1000);
	while (42)
	{
		pthread_mutex_lock(p->info.starvation);
		if (*p->info.end != 0)
		{
			pthread_mutex_unlock(p->info.starvation);
			break ;
		}
		pthread_mutex_unlock(p->info.starvation);
		set_state(p, STR_THINK);
		if ((p->id % 2) == 0)
			take_forks(p, 2);
		else
			take_forks(p, 1);
		if (zzz(p))
			break ;
	}
	return (NULL);
}
