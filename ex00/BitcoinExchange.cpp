/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 16:34:08 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/12 17:23:00 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

std::list<t_record>		BitcoinExchange::_readFile(const std::string &file_name)
{

};

bool					BitcoinExchange::_isDouble(const std::string &str)
{
	bool	find_point = false;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
		{
			if (str[i] == '.' && !find_point)
				find_point = true;
			else if (str[i] == '.' && find_point)
				return (false);
			return (false);
		}
	}
	return (true);
};

bool					_isInt(const std::string &str)
{
	for (unsigned int i = 0; i < str.length(); i++)
		if (!std::isdigit(str[i]))
			return (false);
	return (true);
};

bool					BitcoinExchange::_isValidDate(const std::string &str)
{
	time_t		timestamp_now = std::time(NULL);
	std::tm		*now = std::localtime(&timestamp_now);

	if (str[4] != '-' || str[7] != '-') return (false);

	std::string	year_str = str.substr(0, 4);
	if (!_isInt(year_str)) return (false);
	int			year = std::atoi(year_str.c_str());
	const bool	is_leap_year = year % 4 == 0 || year % 100 != 0 || (year % 100 == 0 && year % 400 == 0);
	if (year > now->tm_year + 1900 || year < _btc_start_year) return (false);

	std::string	month_str = str.substr(5, 2);
	if (!_isInt(month_str)) return (false);
	int			month = std::atoi(month_str.c_str());
	if (month > 11 || month < 0) return (false);

	const int	month_max_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	std::string	day_str = str.substr(8, 2);
	if (!_isInt(day_str)) return (false);
	int			day = std::atoi(day_str.c_str());
	if (day < 0) return (false);
	if (day == 1 && (day > month_max_days[1] + is_leap_year)) return (false);
	if (day > month_max_days[day]) return (false);

	if (year == _btc_start_year && month < _btc_start_month) return (false);
	if (year == _btc_start_year && month == _btc_start_month && day < _btc_start_day) return (false);
	return (true);
};

t_record				BitcoinExchange::_parse_record(const std::string &line, const char separator)
{
	size_t	start_idx = line.find_first_not_of(" ");
	size_t sep_idx = line.find_first_of(separator);
	std::string	first_part = line.substr(start_idx, sep_idx - start_idx);
	first_part = first_part.substr(0, first_part.find_last_not_of(" ") + 1);
	std::string	second_part = line.substr(sep_idx + 1, line.length() - (sep_idx + 1));
	start_idx = second_part.find_first_not_of(" ");
	second_part = second_part.substr(start_idx, second_part.find_last_not_of(" ") - start_idx + 1);
	t_record	record;

	record.line = line;
	record.timestamp = 0;
	record.date = first_part;
	record.is_valid_date = _isValidDate(record.date);
	record.value = 0;
	record.is_valid_value = _isDouble(second_part);
	if (record.is_valid_value)
		record.value = std::strtod(second_part.c_str(), NULL);
};

void					BitcoinExchange::_validateRecord(t_record record)
{
	if (record.value < 0) throw std::runtime_error("Error:  not a positive number.");
	if (record.value > 1000) throw std::runtime_error("Error: too large a number.");
	std::string	error_message = "Error: bad input => ";
	error_message += record.line;
	if (!record.is_valid_date || !record.is_valid_value) throw std::runtime_error(error_message);
};

BitcoinExchange::BitcoinExchange(const std::string &db_name, const std::string &infile_name): _db_name(db_name), _infile_name(infile_name), _db_data{}, _infile_data{} {};
BitcoinExchange::BitcoinExchange(const BitcoinExchange &other): _db_name(other._db_name), _infile_name(other._infile_name), _db_data(other._db_data), _infile_data(other._infile_data) {};
BitcoinExchange::~BitcoinExchange(void) {};
BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this == &other)
		return (*this);
	_db_name = other._db_name;
	_db_data = other._db_data;
	_infile_name = other._infile_name;
	_infile_data = other._infile_data;
	return (*this);
};

void			BitcoinExchange::printStatus(void) const
{
	std::list<t_record>::const_iterator	it = _infile_data.begin();
	while (it != _infile_data.end())
	{
		try
		{
			_validateRecord(*it);
			t_record	found_record = _findRecord(*it);
			double		result = found_record.value * (*it).value;
			std::cout << found_record.date << " => " << (*it).value << " = " << result << std::endl;
			it++;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			it++;
		}
	}

};

t_record		BitcoinExchange::_findRecord(t_record src) const
{
	std::list<t_record>::const_iterator	it = _db_data.begin();
	t_record	last_record;
	while (it != _db_data.end())
	{
		if (src.timestamp == (*it).timestamp)
			return (*it);
		if (last_record.timestamp != -1 && last_record.timestamp < src.timestamp && (*it).timestamp > src.timestamp)
			return (last_record);
		last_record = *it;
		it++;
	}
	std::string	error_message = "Error: bad input => ";
	error_message += src.date;
	throw std::runtime_error(error_message);
};
