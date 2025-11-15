/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:46:20 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/14 15:23:08 by rdellaza         ###   ########.fr       */
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
	pthread_mutex_t	meal_mutex;	// Added to try to solve issues
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

/* =============== UTILS =============== */
long	get_time(void);
void	ft_usleep(long milliseconds);
void	print_status(t_philo *philo, char *status);

/* =============== ROUTINE =============== */
void	*philosopher_routine(void *arg);
int		create_threads(t_data *data);
int		join_threads(t_data *data);

/* =============== ACTIONS =============== */
int		take_forks(t_philo *philo);
void	drop_forks(t_philo *philo);
void	philo_eat(t_philo *philo);
void	philo_sleep(t_philo *philo);
void	philo_think(t_philo *philo);

/* =============== MONITOR =============== */
void	*monitor_routine(void *arg);
int		check_death(t_data *data);
int		is_simulation_over(t_data *data);
int		check_all_ate(t_data *data);

/* =============== CLEANUP =============== */
void	cleanup_mutexes(t_data *data);
void	cleanup_data(t_data *data);

#endif
