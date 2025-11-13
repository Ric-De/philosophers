/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 00:40:25 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 11:24:02 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	params_are_valids(t_general *data, char **argv)
{
	if (check_valid_format(argv) == 0)
		return (0);
	if (check_size_int(argv) == 0)
		return (0);
	params_conversion_to_int(data, argv);
	if (check_extreme_cases(data) == 0)
		return (0);
	return (1);
}
