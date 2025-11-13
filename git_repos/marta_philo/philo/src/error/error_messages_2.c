/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_messages_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfontser <mfontser@student.42.barcel>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 16:35:18 by mfontser          #+#    #+#             */
/*   Updated: 2024/12/03 17:59:31 by mfontser         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	error_max_int(void)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Error: max int (2147483647) ", 28);
	write(2, "has been exceeded\n", 18);
	write(2, END, ft_strlen(END));
}

void	error_non_numbers_found(void)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Error: Only positive numbers accepted", 37);
	write(2, " (without spaces)\n", 19);
	write(2, END, ft_strlen(END));
}

void	error_positive_sign(void)
{
	write(2, RED, ft_strlen(RED));
	write(2, "Error: There is only one plus sign, ", 36);
	write(2, "incorrect format\n", 17);
	write(2, END, ft_strlen(END));
}
