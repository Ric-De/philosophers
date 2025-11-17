/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 02:41:30 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 02:41:35 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_forks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 01:00:00 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 01:00:00 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Select fork order based on global ordering strategy
** Last philosopher takes right fork first to break circular wait
** All others take left fork first (lower numbered fork)
*/
void	select_fork_order(t_philo *philo, pthread_mutex_t **first,
		pthread_mutex_t **second)
{
	if (philo->id == philo->data->nb_philos)
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
	else
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
}

/*
** Lock first fork and check if simulation ended
** Returns 1 if successful, 0 if simulation ended
*/
int	lock_first_fork(t_philo *philo, pthread_mutex_t *first_fork)
{
	pthread_mutex_lock(first_fork);
	print_status(philo, "has taken a fork");
	if (is_simulation_over(philo->data))
	{
		pthread_mutex_unlock(first_fork);
		return (0);
	}
	return (1);
}
