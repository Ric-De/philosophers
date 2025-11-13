/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ph_struct.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <deordone@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 13:29:22 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 13:06:39 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PH_STRUCT_H
# define PH_STRUCT_H
# include "philosophers.h"

typedef pthread_mutex_t	t_fork;

typedef struct s_info
{
	long				n_philo;
	long				time2_die;
	long				time2_eat;
	long				time2_sleep;
	long				max_eat;
	int					*end;
	t_fork				*starvation;
	t_fork				*printor;
	t_fork				*time;
	t_fork				*pleased;
}						t_info;

typedef struct s_philo
{
	int					id;
	int					state;
	int					foods;
	long				last_noodle;
	pthread_t			thread_id;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_info				info;
}						t_philo;

typedef struct s_table
{
	t_fork				*f;
	t_philo				*p;
	t_info				info;
}						t_table;

#endif
