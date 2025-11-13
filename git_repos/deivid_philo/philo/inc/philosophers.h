/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcel>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/13 10:28:14 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 14:01:10 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include "ph_macros.h"
# include "ph_struct.h"

/* MAIN */
int		destroy_all(t_table *table, int i);

/* INIT */
int	init_spider(t_table *table);
int	init_philo(t_table *table);
int	init_mutex(t_table *table);

/* PARSER */
int		parser(int ac, char **av, t_table *table);

/* DELFOS */

int		memento_mori(t_philo *p);
void	*the_last_supper(void *arg);

/* AUX */
void	set_state(t_philo *p, char *state);
time_t	get_mstime(void);
long	get_time_value(void);
void	ft_usleep(int ms);

#endif
