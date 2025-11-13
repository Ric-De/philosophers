/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 22:08:12 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 17:37:10 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	get_current_time(void)
{
	struct timeval	tmp;
	unsigned int	current_time;

	gettimeofday(&tmp, NULL);
	current_time = (unsigned int)tmp.tv_sec * 1000 + tmp.tv_usec * 0.001;
	return (current_time);
}

unsigned int	get_simulation_time(t_general *data)
{
	return (get_current_time() - data->t_start);
}

void	suspend(t_philo *philo, unsigned int time)
{
	unsigned int	start_time;

	start_time = get_simulation_time(philo->data);
	while (get_simulation_time(philo->data) - start_time < time
		&& check_simulation_state(philo) == RUNNING)
		usleep(100);
}
