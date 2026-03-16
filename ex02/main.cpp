/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 21:43:54 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/15 23:34:32 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

int main(int argc, char **argv)
{
	if (argc == 1)
	{
		std::cerr << "Argument Error: try " << argv[0] << " <number_1> <number_2> ... <number_n> " << std::endl;
		return (1);
	}
	try
	{
		PmergeMe merge((const char **)(argv + 1), argc - 1);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

}
