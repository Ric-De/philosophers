/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rdellaza <rdellaza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 02:41:55 by rdellaza          #+#    #+#             */
/*   Updated: 2025/11/17 03:55:06 by rdellaza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philo.h"

/*
** ft_atoi
** -------
** Converts a string representation of a number to an integer.
** (ASCII to Integer)
** 
** Flow:
** 1. Skip leading whitespace (spaces, tabs, newlines, etc.)
** 2. Check for optional sign (+ or -)
** 3. Convert digit characters to integer value
** 4. Apply sign and return result
** 
** Parameters:
**   - str: string to convert (e.g., "42", "-123", "  +99")
** 
** Returns:
**   - Integer value of the string
** 
** Handles:
**   - Leading whitespace (space, tab, newline, etc.)
**   - Optional '+' or '-' sign
**   - Multiple digits
** 
** Example:
**   ft_atoi("  -42")  -> -42
**   ft_atoi("123")    -> 123
**   ft_atoi("+99")    -> 99
*/
int	ft_atoi(const char *str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	
	/* Skip leading whitespace characters
	** ASCII values: space=32, tab=9, newline=10, vertical tab=11,
	**               form feed=12, carriage return=13 */
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	
	/* Check for optional sign (+ or -) */
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;  /* Negative number */
		str++;  /* Move past the sign character */
	}
	
	/* Convert digit characters to integer
	** Each digit is converted by: (char_value - '0')
	** Then we shift previous digits left by multiplying by 10
	** Example: "123" -> 0*10+1=1 -> 1*10+2=12 -> 12*10+3=123 */
	while (*str >= '0' && *str <= '9')
	{
		result = result * 10 + (*str - '0');
		str++;
	}
	
	/* Apply the sign and return final result */
	return (result * sign);
}

/*
** is_valid_number
** ---------------
** Validates that a string contains only digit characters (0-9).
** Used to ensure command line arguments are properly formatted numbers.
** 
** Parameters:
**   - str: string to validate
** 
** Returns:
**   - 1 if string is valid (non-empty and contains only digits)
**   - 0 if string is invalid (NULL, empty, or contains non-digit characters)
** 
** This function ensures:
**   - String is not NULL
**   - String is not empty
**   - Every character is a digit (0-9)
**   - No signs (+/-), no decimals, no whitespace
** 
** Valid examples:
**   "42"     -> 1 (valid)
**   "123"    -> 1 (valid)
**   "0"      -> 1 (valid)
** 
** Invalid examples:
**   "-42"    -> 0 (contains minus sign)
**   "12.5"   -> 0 (contains decimal point)
**   "12a"    -> 0 (contains letter)
**   ""       -> 0 (empty string)
**   NULL     -> 0 (null pointer)
*/
int	is_valid_number(char *str)
{
	int	i;

	i = 0;
	
	/* Check for NULL or empty string */
	if (!str || !str[0])
		return (0);
	
	/* Check each character in the string */
	while (str[i])
	{
		/* If any character is not a digit, the string is invalid */
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	
	/* All characters are digits, string is valid */
	return (1);
}
