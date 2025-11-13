/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos_struct.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 16:44:21 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 12:59:19 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	define_color(t_philo *philo)
{
	if (philo->id % 10 == 0)
		philo->color = BLUE;
	else if (philo->id % 10 == 1)
		philo->color = GREEN;
	else if (philo->id % 10 == 2)
		philo->color = FUCSIA;
	else if (philo->id % 10 == 3)
		philo->color = ORANGE;
	else if (philo->id % 10 == 4)
		philo->color = CYAN;
	else if (philo->id % 10 == 5)
		philo->color = PINK;
	else if (philo->id % 10 == 6)
		philo->color = YELLOW;
	else if (philo->id % 10 == 7)
		philo->color = LIME_GREEN;
	else if (philo->id % 10 == 8)
		philo->color = PURPLE;
	else if (philo->id % 10 == 9)
		philo->color = TURQUOISE;
}

void	init_philos_struct(t_general *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		data->philos[i].right_fork = &data->forks_array[i];
		if (i > 0)
			data->philos[i].left_fork = data->philos[i - 1].right_fork;
		data->philos[i].id = i + 1;
		data->philos[i].data = data;
		data->philos[i].n_meals = &data->num_meals_array[i];
		data->philos[i].num_meals = 0;
		data->philos[i].l_meal = &data->last_meal_array[i];
		data->philos[i].last_meal = 0;
		define_color(&data->philos[i]);
		i++;
	}
	if (data->philo_num > 1)
		data->philos[0].left_fork = data->philos[i - 1].right_fork;
	else
		data->philos[0].left_fork = data->philos[0].right_fork;
}
