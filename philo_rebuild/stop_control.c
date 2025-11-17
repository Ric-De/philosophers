/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 17:11:20 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/16 17:11:35 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** Check if this philosopher should stop
** Uses personal stop flag instead of global flag
** Returns 1 if should stop, 0 if should continue
*/
int	should_philosopher_stop(t_philo *philo)
{
	int	result;

	pthread_mutex_lock(&philo->stop_mutex);
	result = philo->should_stop;
	pthread_mutex_unlock(&philo->stop_mutex);
	return (result);
}

/*
** Tell ALL philosophers to stop by setting each personal flag
** This ensures all philosophers stop at the same checkpoint
** Called by monitor when simulation should end
*/
void	notify_all_philosophers(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_lock(&data->philos[i].stop_mutex);
		data->philos[i].should_stop = 1;
		pthread_mutex_unlock(&data->philos[i].stop_mutex);
		i++;
	}
}
