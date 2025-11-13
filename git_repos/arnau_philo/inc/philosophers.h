/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:17:03 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/25 20:18:26 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <sys/time.h>
# include <stdbool.h>
# include <limits.h>

# define USAGE "<number_of_philos> <time_to_die> <time_to_eat> <time_to_sleep>"
# define OPTIONAL " <number_times_each_philosopher_must_eat(optional)>"
# define USAGE_ERROR 0
# define INPUT_ERROR 1
# define MEMORY_ERROR 2

# define THINK 0
# define LEFT_FORK 1
# define RIGHT_FORK 2
# define EAT 3
# define SLEEP 4

# define RO "\033[31m"
# define VE "\033[32m"
# define AM "\033[33m"
# define AZ "\033[34m"
# define R "\033[0m"

typedef struct s_data	t_data;

typedef struct s_philos
{
	int		id;
	int		right_fork;
	int		left_fork;
	int		meals_count;
	time_t	last_time_meal;
	t_data	*data;
}	t_philos;

typedef struct s_data
{
	int					n_philos;
	time_t				t_die;
	time_t				t_eat;
	time_t				t_sleep;
	int					meals;
	int					meals_flag;
	int					full_count;
	bool				end_flag;
	time_t				start_time;
	pthread_t			*philo;
	pthread_mutex_t		meal_mutex;
	pthread_mutex_t		end_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		*forks_mutex;
	t_philos			**philos;
}	t_data;

void	output(char *s1, char *s2, char *s3);
void	error_exit(int type, t_data *data);
void	check_input(t_data *data, int flag);
void	init_pthreads(t_data *data);
time_t	get_time(void);
int		ft_atoi(char *str);
void	*philo_routine(void *arg);
void	*monitor_health(void *arg);
void	run_philos(t_data *data);
void	print_activity(t_philos *philo, int i);
int		check_end(t_philos *philo);
int		ft_usleep(time_t time);
void	check_syntax(t_data *data, char **av);
void	parsing(t_data *data, int ac, char **av);
void	free_resources(t_data *data);
void	ph_delay(time_t t_start);
int		check_end(t_philos *philo);
int		init_struct(int ac, char **av);
void	take_left_fork(t_philos *philo);
void	take_right_fork(t_philos *philo);

#endif
