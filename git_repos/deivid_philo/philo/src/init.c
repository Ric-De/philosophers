/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:06:03 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 14:27:51 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_spider(t_table *table)
{
	t_philo	*p;
	int		philos;
	int		i;

	philos = (int)table->info.n_philo;
	p = table->p;
	i = -1;
	get_mstime();
	while (++i < philos)
	{
		if (pthread_create(&(p[i].thread_id), NULL, the_last_supper,
				&p[i]) != 0)
			return (destroy_all(table, philos + AUX_MUTEX));
	}
	i = -1;
	memento_mori(p);
	while (++i < philos)
	{
		if (pthread_join(p[i].thread_id, NULL) != 0)
			return (destroy_all(table, philos + AUX_MUTEX));
	}
	return (0);
}

static int	everyone_eat(t_philo *p)
{
	int	i;

	i = 0;
	pthread_mutex_lock(p->info.pleased);
	while (p[i].state == 1)
		i++;
	if (i == p->info.n_philo)
	{
		pthread_mutex_unlock(p->info.pleased);
		printf("Every Philo is Satisfied 😋\n");
		return (-1);
	}
	pthread_mutex_unlock(p->info.pleased);
	return (0);
}

int	memento_mori(t_philo *p)
{
	int	i;

	i = 0;
	while (42)
	{
		if (everyone_eat(p) < 0)
			break ;
		pthread_mutex_lock(p->info.time);
		if (p[i].last_noodle + p[i].info.time2_die < get_mstime())
		{
			pthread_mutex_unlock(p->info.time);
			set_state(&p[i], STR_DIE);
			pthread_mutex_lock(p->info.starvation);
			*p[i].info.end = 1;
			pthread_mutex_unlock(p->info.starvation);
			break ;
		}
		pthread_mutex_unlock(p->info.time);
		i++;
		if (i >= p[i].info.n_philo)
			i = 0;
	}
	return (-1);
}

int	init_philo(t_table *table)
{
	int	philos;
	int	i;

	philos = (int)table->info.n_philo;
	i = -1;
	table->p = malloc(sizeof(t_philo) * philos);
	if (!table->p)
		return (destroy_all(table, philos + AUX_MUTEX));
	while (++i < philos)
	{
		table->p[i].id = i;
		if (i != 0)
			table->p[i].left_fork = table->p[i - 1].right_fork;
		table->p[i].right_fork = table->f + i;
		table->p[i].foods = 0;
		table->p[i].state = UNSATISFIED;
		table->p[i].info = table->info;
		table->p[i].info.starvation = table->f + philos;
		table->p[i].info.printor = table->f + philos + 1;
		table->p[i].info.time = table->f + philos + 2;
		table->p[i].info.pleased = table->f + philos + 3;
	}
	table->p[0].left_fork = table->f + (philos - 1);
	return (0);
}

int	init_mutex(t_table *table)
{
	int	philos;
	int	i;

	philos = table->info.n_philo;
	table->f = malloc(sizeof(t_fork) * philos + AUX_MUTEX);
	if (!table->f)
		return (destroy_all(table, -1));
	i = -1;
	while (++i < philos)
	{
		if (pthread_mutex_init(&(table->f[i]), NULL) != 0)
			return (destroy_all(table, i));
	}
	pthread_mutex_init(&(table->f[i]), NULL);
	pthread_mutex_init(&(table->f[i + 1]), NULL);
	pthread_mutex_init(&(table->f[i + 2]), NULL);
	pthread_mutex_init(&(table->f[i + 3]), NULL);
	return (0);
}
