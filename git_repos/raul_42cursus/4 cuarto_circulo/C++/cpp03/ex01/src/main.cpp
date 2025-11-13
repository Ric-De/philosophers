/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rmanzana <rmanzana@student.42barcelona.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/10 20:46:53 by rmanzana          #+#    #+#             */
/*   Updated: 2025/10/30 18:48:57 by rmanzana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ClapTrap.hpp"
#include "../inc/ScavTrap.hpp"

int	main(void)
{
	ClapTrap dog("dog");
	ClapTrap cat("cat");
	ClapTrap nobody;

	dog.attack("cat");
	cat.takeDamage(5);
	cat.beRepaired(5);
	dog.takeDamage(5);
	dog.takeDamage(5);
	dog.beRepaired(5);

	std::cout << std::endl
	<< "\033[1;31m=================================================="
	<< std::endl
	<< "\033[33m		now with ScavTrap"
	<< std::endl
	<< "\033[31m==================================================\033[0m"
	<< std::endl << std::endl;

	ScavTrap sheep("sheep");
	ScavTrap wolf("wolf");
	ScavTrap noone;

	sheep.attack("wolf");
	wolf.takeDamage(20);
	wolf.beRepaired(15);
	wolf.attack("sheep");
	for (int i = 0; i < 5; i++)
		sheep.takeDamage(20);
	sheep.beRepaired(5);
	wolf.guardGate();

	return (0);
}

