/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 02:34:03 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/14 18:35:56 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <string>
# include <list>
# include <stdexcept>
# include <ctime>
# include <fstream>
# include <iostream>
# include <algorithm>
# include <cstring>

# define DATA_FILE_NAME "data.csv"

typedef struct s_record	t_record;
struct s_record
{
	std::string			line;
	std::string			date;
	bool				is_valid_date;
	time_t				timestamp;
	double				value;
	bool				is_valid_value;
	bool				is_valid_input;
};


class BitcoinExchange
{
	private:
		std::string			_db_name;
		std::string			_infile_name;
		std::list<t_record>	_db_data;
		std::list<t_record>	_infile_data;

		static const int	_btc_start_year = 2009;
		static const int	_btc_start_month = 0;
		static const int	_btc_start_day = 2;

		static std::list<t_record>	_readFile(const std::string &file_name, const char separator);
		static t_record				_parseRecord(const std::string &line, const char separator);
		static void					_validateRecord(t_record record, bool validate_span);
		static bool					_isDouble(const std::string &str);
		static bool					_isInt(const std::string &str);
		static bool					_isValidDate(const std::string &str);
		static tm					_makeDate(const std::string &str);
		t_record					_findRecord(t_record src) const;

	public:
		BitcoinExchange(const std::string &db_name, const std::string &infile_name);
		BitcoinExchange(const BitcoinExchange &other);
		~BitcoinExchange(void);
		BitcoinExchange	&operator=(const BitcoinExchange &other);

		void			printStatus(void) const;

};

#endif
