/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:46:20 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/13 16:28:15 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

/* TODO: More structs and the whole prototypes as I build the thing */

/* =============== STRUCTURES =============== */

typedef struct	s_data
{
	int	nb_philos;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	must_eat_count;
	long	start_time;
	int	someone_died;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	pthread_mutex_t	*forks;
	struct s_philo	*philos;
}	t_data;

typedef struct	s_philo
{
	int	id;
	int	meals_eaten;
	long	last_meal_time;
	pthread_t	thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_data	*data;
}	t_philo;

/* =============== FUNCTION PROTOTYPES =============== */

/* TODO: For function prototypes I create them */

/* =============== INITIALIZATION =============== */
int		init_data(t_data *data);
int		init_mutexes(t_data *data);
int		init_forks(t_data *data);
int		init_philosophers(t_data *data);

/* =============== PARSING =============== */
int		parse_arguments(int argc, char **argv, t_data *data);
int		ft_atoi(const char *str);
int		is_valid_number(char *str);

#endif
