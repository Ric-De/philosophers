/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: deordone <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 13:06:08 by deordone          #+#    #+#             */
/*   Updated: 2024/06/20 14:31:02 by deordone         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	fill_info(int ac, char **av, t_info *info)
{
	info->n_philo = atol(av[1]);
	info->time2_die = atol(av[2]);
	info->time2_eat = atol(av[3]);
	info->time2_sleep = atol(av[4]);
	if (ac == 6)
		info->max_eat = atol(av[5]);
	else
		info->max_eat = -1;
	info->end = malloc(sizeof(int) * 1);
	info->end[0] = 0;
}

static int	is_num(char **num)
{
	int	i;
	int	j;

	i = 1;
	while (num[i])
	{
		j = 0;
		while (num[i][j])
		{
			if (num[i][j] > 47 && num[i][j] < 58)
				j++;
			else
				return (-1);
		}
		i++;
	}
	return (0);
}

static int	is_max(t_info *data)
{
	if (!(data->n_philo <= INT_MAX && data->n_philo >= INT_MIN))
		return (-1);
	if (!(data->time2_die <= INT_MAX && data->time2_die >= INT_MIN))
		return (-1);
	if (!(data->time2_eat <= INT_MAX && data->time2_eat >= INT_MIN))
		return (-1);
	if (!(data->time2_sleep <= INT_MAX && data->time2_sleep >= INT_MIN))
		return (-1);
	if (data->max_eat != -1 && !(data->max_eat <= INT_MAX
			&& data->max_eat >= INT_MIN))
		return (-1);
	return (0);
}

int	parser(int ac, char **av, t_table *table)
{
	t_info	info;

	if (is_num(av) < 0)
		return (-1);
	fill_info(ac, av, &info);
	if (is_max(&info) < 0)
		return (-1);
	table->info = info;
	if (table->info.n_philo <= 0)
		return (-1);
	return (0);
}
