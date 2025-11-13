/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:19:25 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/24 15:57:02 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

void	output(char *s1, char *s2, char *s3)
{
	printf("%s", s1);
	printf("%s", s2);
	printf("%s\n", s3);
}

void	error_exit(int type, t_data *data)
{
	if (type == 0)
		output("Usage: ", USAGE, OPTIONAL);
	else if (type == 1)
		output("Invalid input", "", "");
	else if (type == 2)
		output("Error allocating memory", "", "");
	if (data)
		free(data);
	exit (1);
}

void	check_input(t_data *data, int flag)
{
	if (data->n_philos <= 0 || data->n_philos > 200)
		error_exit(INPUT_ERROR, data);
	if (data->t_die < 60 || data->t_die > INT_MAX)
		error_exit(INPUT_ERROR, data);
	if (data->t_eat < 60 || data->t_die > INT_MAX)
		error_exit(INPUT_ERROR, data);
	if (data->t_sleep < 60 || data->t_die > INT_MAX)
		error_exit(INPUT_ERROR, data);
	if (flag)
	{
		if (data->meals <= 0 || data->t_die > INT_MAX)
			error_exit(INPUT_ERROR, data);
	}
}
