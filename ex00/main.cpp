/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 17:39:02 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/14 17:43:36 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Argument Error: try -> " << argv[0] << " <infile_name>" << std::endl;
		return (1);
	}
	try
	{
		const BitcoinExchange	btc(DATA_FILE_NAME, argv[1]);
		btc.printStatus();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}

}
