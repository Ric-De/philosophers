/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 17:31:17 by jaacosta          #+#    #+#             */
/*   Updated: 2025/05/09 18:31:54 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

/***	Librerias usadas/Used Libraries		***/
# include <unistd.h>
# include <stdlib.h>
# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

/***	Estructuras de datos/Data structures	***/
typedef struct s_data	t_data;

typedef struct s_data
{
	long long		start_time;
	pthread_mutex_t	stop_mutex;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	int				num_philos;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				must_eat_count;
	int				stop_flag;	
	struct s_philo	*philos;
}			t_data;

typedef struct s_philo
{
	int				id;
	long long		last_meal;
	int				meals_eaten;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_data			*shared_data;
}			t_philo;

/***	Funciones Auxiliares/Auxiliar Function	***/
void		update_last_meal(t_philo *philo);
/***	Funciones Auxiliares/Auxiliar Function	***/
long		ft_atoi(const char *nptr);
void		clean(t_data *data, t_philo *philos);
long long	get_time(void);
int			ft_usleep(time_t time);
void		precise_usleep(long long miliseconds, t_data *data);
void		print_status(t_philo *philo, char *str);
void		print_status_live(t_philo *philo, char *str);
/***	Parseo de datos/data parser	***/
int			validate_av(int ac, char **av);
/***	Inicializaciones datos/data inits	***/
t_data		init_data(int ac, char **av);
t_philo		*init_philos(t_data *data);
void		init_mutexes(t_data *data);
/***	Rutina filosofos/Philosopher routine	***/
void		eat(t_philo *philo);
void		sleep_philo(t_philo *philo);
void		think_philo(t_philo *philo);
void		release_forks(t_philo *philo);
/***	Monitor y verificadores/Checkers and monitor	***/
int			check_meals(t_data *data);
long long	get_last_meal(t_philo *philo);
void		set_stop(t_data *data);
void		*monitor_routine(void *arg);
int			check_stop(t_data *data);
/***	Logica del filosofo/Philosopher Logic	***/
void		*philosopher(void *arg);

#endif
