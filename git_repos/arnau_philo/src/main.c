/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arcebria <arcebria@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 19:09:08 by arcebria          #+#    #+#             */
/*   Updated: 2025/02/24 18:04:41 by arcebria         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/philosophers.h"

int	main(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		error_exit (USAGE_ERROR, NULL);
	if (init_struct(ac, av))
		error_exit(MEMORY_ERROR, NULL);
	return (0);
}
