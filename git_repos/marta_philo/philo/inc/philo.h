/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/06 12:40:28 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/12 11:11:02 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/*COLORS*/
# define END "\x1b[0m"
# define RED "\e[1;91m"
# define GREEN "\e[1;92m"
# define YELLOW "\e[1;93m"
# define BLUE "\e[1;94m"
# define TURQUOISE "\e[1;38;5;80m"
# define PINK "\e[1;38;5;213m"
# define FUCSIA "\e[1;38;5;201m"
# define PURPLE "\e[1;38;5;135m"
# define CYAN "\e[1;96m"
# define LIME_GREEN "\e[1;38;5;118m"
# define ORANGE "\e[1;38;2;255;128;0m"

# define MAX_INT "2147483647"

// SIMULATION STATE
# define RUNNING 1
# define STOPPED 0

// PRINT STATES
# define TAKEN_FORK_STATE "has taken a fork 🤗🍽️"
# define EATING_STATE "is eating 🤤🍝"
# define SLEEPING_STATE "is sleeping 😴💤"
# define THINKING_STATE "is thinking 🤔💭"
# define DIED "died 😵💀"

typedef struct s_philo
{
	int					id;
	char				*color;
	pthread_t			thread_id;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*n_meals;
	int					num_meals;
	pthread_mutex_t		*l_meal;
	unsigned int		last_meal;
	struct s_general	*data;
}						t_philo;

typedef struct s_general
{
	int					philo_num;
	t_philo				*philos;
	int					stop_simulation;
	pthread_mutex_t		*forks_array;
	pthread_mutex_t		*dead_status_array;
	pthread_mutex_t		*num_meals_array;
	pthread_mutex_t		*last_meal_array;
	pthread_mutex_t		start_lock;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		life_checker_lock;
	unsigned int		t_start;
	unsigned int		time_to_die;
	int					time_to_eat;
	int					time_to_sleep;
	int					num_eats_each_philo;
	int					num_threads;

}						t_general;

// PARSING
int						params_are_valids(t_general *data, char **argv);
int						check_valid_format(char **argv);
int						check_size_int(char **argv);
int						check_signs(char **argv, int i, int *j);
int						params_conversion_to_int(t_general *data, char **argv);
int						check_extreme_cases(t_general *data);

// INITIALITATIONS
int						initialitations(t_general *data);
int						init_dead_status_array(t_general *data);
int						init_num_meals_array(t_general *data);
int						init_last_meal_array(t_general *data);
int						init_data_struct(t_general *data);
void					init_philos_struct(t_general *data);
void					define_color(t_philo *philo);

// BUILD PHILOS AND FORKS
int						build_philos_and_forks(t_general *data);
int						build_philos_array(t_general *data);
int						build_forks_array(t_general *data);

// SIMULATION
void					run_simulation(t_general *data);
void					create_philos_threads(t_general *data);
void					*philo_routine(t_philo *philo);
int						check_simulation_state(t_philo *philo);
void					supervise_simulation(t_general *data);
void					kill_philo(t_general *data, t_philo *philo);
int						all_meals_eaten(t_general *data,
							int num_of_philos_eaten_enough);
unsigned int			get_current_time(void);
unsigned int			get_simulation_time(t_general *data);
void					suspend(t_philo *philo, unsigned int time);
int						take_forks(t_philo *philo);
int						eat(t_philo *philo);
void					unlock_forks(t_philo *philo);
int						sleep_nap(t_philo *philo);
int						think(t_philo *philo);
int						print_state(t_philo *philo, char *state);
void					finish_simulation(t_general *data);

// ERROR_MESSAGES
void					error_max_int(void);
void					wrong_number_of_parameters(void);
void					error_empty_string(void);
void					error_negative_sign(void);
void					error_positive_sign(void);
void					error_non_numbers_found(void);
void					error_space_after_sign(void);

// FREE
void					free_2_last_mutex(t_general *data);
void					free_3_last_mutex(t_general *data);
void					free_since_last_meal_function(t_general *data);
void					free_since_num_meals_function(t_general *data);
void					free_since_build_philos_function(t_general *data);
void					free_all_data(t_general *data);

// LIBF UTILS
size_t					ft_strlen(const char *str);
int						ft_strncmp(const char *s1, const char *s2, size_t n);
int						ft_isspace(int c);
int						ft_atoi(const char *str);

#endif