/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 23:28:11 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:07:52 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_state(t_philo *philo, char *state)
{
	char	*color;

	color = philo->color;
	pthread_mutex_lock(&philo->data->write_lock);
	if (check_simulation_state(philo) == RUNNING)
	{
		printf("%s[%d]  %d %s\n" END, color, get_simulation_time(philo->data),
			philo->id, state);
		pthread_mutex_unlock(&philo->data->write_lock);
		return (RUNNING);
	}
	pthread_mutex_unlock(&philo->data->write_lock);
	return (STOPPED);
}
