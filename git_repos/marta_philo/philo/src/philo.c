/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:07:55 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 18:20:19 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	t_general	data;

	if (argc > 6 || argc < 5)
	{
		wrong_number_of_parameters();
		return (1);
	}
	if (params_are_valids(&data, argv) == 0)
		return (1);
	if (initialitations(&data) == 0)
		return (1);
	run_simulation(&data);
	finish_simulation(&data);
	return (0);
}
