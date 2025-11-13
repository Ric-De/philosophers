/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 17:27:11 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/24 17:27:40 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

time_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	ft_usleep(time_t time)
{
	time_t	elapsed;
	time_t	start_time;

	start_time = get_time();
	while (1)
	{
		elapsed = get_time() - start_time;
		if (elapsed >= time)
			break ;
		usleep(500);
	}
	return (1);
}

void	ph_delay(time_t t_start)
{
	while (get_time() < t_start)
		continue ;
}
