/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_valid_format_and_size.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 11:20:35 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 18:10:52 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_signs(char **argv, int i, int *j)
{
	if (argv[i][*j] == '-')
	{
		error_negative_sign();
		return (0);
	}
	if (argv[i][*j] == '+')
	{
		if (argv[i][*j + 1] == '\0')
		{
			error_positive_sign();
			return (0);
		}
		(*j)++;
	}
	return (1);
}

int	check_size_int(char **argv)
{
	int		i;
	char	*to_check;

	i = 1;
	while (argv[i])
	{
		to_check = argv[i];
		if (argv[i][0] == '+')
			to_check++;
		while (*to_check == '0' && to_check[1] != '\0')
			to_check++;
		if (ft_strlen(to_check) > 10)
			return (error_max_int(), 0);
		else if (ft_strlen(to_check) == 10)
		{
			if (argv[i][0] != '-' && ft_strncmp(MAX_INT, to_check, 10) < 0)
				return (error_max_int(), 0);
		}
		i++;
	}
	return (1);
}

int	check_valid_format(char **argv)
{
	int	i;
	int	j;

	i = 0;
	while (argv[++i])
	{
		j = 0;
		if (argv[i][j] == '\0')
		{
			error_empty_string();
			return (0);
		}
		if (check_signs(argv, i, &j) == 0)
			return (0);
		while (argv[i][j] != '\0')
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
			{
				error_non_numbers_found();
				return (0);
			}
			j++;
		}
	}
	return (1);
}
