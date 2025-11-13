/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jaacosta <jaacosta@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 16:39:45 by jaacosta          #+#    #+#             */
/*   Updated: 2025/05/08 19:28:17 by jaacosta         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philo.h"

long	ft_atoi(const char *nptr)
{
	long	y;
	long	z;

	y = 1;
	z = 0;
	while ((*nptr >= 9 && *nptr <= 13) || *nptr == 32)
		nptr++;
	if (*nptr == 45)
		y *= -1;
	if (*nptr == 43 || *nptr == 45)
		nptr++;
	while (*nptr >= '0' && *nptr <= '9')
	{
		z = (*nptr - '0') + (z * 10);
		nptr++;
	}
	return (z * y);
}

int	ft_isdigit(int c)
{
	if (c > 47 && c < 58)
		return (1);
	else
		return (0);
}

int	is_positive_number(char *str)
{
	int	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	validate_av(int ac, char **av)
{
	int	i;

	if (ac < 5 || ac > 6)
	{
		printf("Error: Nro de argumentos incorrectos");
		return (0);
	}
	i = 1;
	while (i < ac)
	{
		if (!is_positive_number(av[i]) || ft_atoi(av[i]) < 1)
		{
			printf("Error: Argvs deben ser enteros positivos mayores que cero");
			return (0);
		}
		i++;
	}
	if (ft_atoi(av[1]) < 1)
	{
		printf("Error: Debe haber al menos un filosofo");
		return (0);
	}
	return (1);
}
