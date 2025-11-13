/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish_simulation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 17:33:54 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:34:13 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	finish_simulation(t_general *data)
{
	int	i;

	i = 0;
	while (i < data->num_threads)
	{
		pthread_join(data->philos[i].thread_id, NULL);
		i++;
	}
	free_all_data(data);
}
