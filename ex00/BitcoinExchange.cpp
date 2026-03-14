/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 16:34:08 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/14 19:00:55 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

std::list<t_record>		BitcoinExchange::_readFile(const std::string &file_name, const char separator)
{
	if (file_name.empty()) throw std::runtime_error("File Error: 'file name' is empty!");
	std::ifstream	file(file_name.c_str());

	if (!file.is_open()) throw std::runtime_error("File Error: cannot open file!");
	std::list<t_record>	record_list;
	t_record			record;
	std::string			line;
	std::getline(file, line);
	while (!file.eof())
	{
		std::getline(file, line);
		if (line.empty())
			continue ;
		record = _parseRecord(line, separator);
		record_list.push_back(record);
	}
	return (record_list);
};

bool					BitcoinExchange::_isDouble(const std::string &str)
{
	bool	find_point = false;
	bool	found_signal = false;

	for (unsigned int i = 0; i < str.length(); i++)
	{
		if (!std::isdigit(str[i]))
		{
			if (!found_signal && (str[i] == '+' || str[i] == '-'))
			{
				found_signal = true;
				continue ;
			}
			if (str[i] == '.' && !find_point)
			{
				find_point = true;
				continue ;
			}
			return (false);
		}
	}
	return (true);
};

bool					BitcoinExchange::_isInt(const std::string &str)
{
	bool	found_signal = false;

	for (unsigned int i = 0; i < str.length(); i++)
		if (!std::isdigit(str[i]))
		{
			if (!found_signal && (str[i] == '+' || str[i] == '-'))
			{
				found_signal = true;
				continue ;
			}
			return (false);
		}
	return (true);
};

bool					BitcoinExchange::_isValidDate(const std::string &str)
{
	time_t		timestamp_now = std::time(NULL);
	std::tm		*now = std::localtime(&timestamp_now);
	std::tm		str_time = _makeDate(str);
	if (str_time.tm_year < 0 || str_time.tm_mon < 0 || str_time.tm_mday < 0) return (false);

	int			year = str_time.tm_year + 1900;
	if (str_time.tm_year > now->tm_year || year < _btc_start_year) return (false);

	if (str_time.tm_mon > 11) return (false);

	const int	month_max_days[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	const bool	is_leap_year = year % 4 == 0 || (year % 100 == 0 && year % 400 == 0) || year % 100 != 0;

	if (str_time.tm_mon == 1 && (str_time.tm_mday > month_max_days[1] + is_leap_year)) return (false);
	if (str_time.tm_mday > month_max_days[str_time.tm_mon]) return (false);

	if (year == _btc_start_year && str_time.tm_mon < _btc_start_month) return (false);
	if (year == _btc_start_year && str_time.tm_mon == _btc_start_month && str_time.tm_mday < _btc_start_day) return (false);
	return (true);
};

t_record				BitcoinExchange::_parseRecord(const std::string &line, const char separator)
{
	t_record	record = t_record();
	record.line = line;
	size_t	start_idx = line.find_first_not_of(" ");
	size_t sep_idx = line.find_first_of(separator);
	record.is_valid_input = sep_idx != std::string::npos;
	if (!record.is_valid_input) return (record);
	std::string	date_part = line.substr(start_idx, sep_idx - start_idx);
	date_part = date_part.substr(0, date_part.find_last_not_of(" ") + 1);
	std::string	number_part = line.substr(sep_idx + 1, line.length() - (sep_idx + 1));
	start_idx = number_part.find_first_not_of(" ");
	number_part = number_part.substr(start_idx, number_part.find_last_not_of(" ") - start_idx + 1);

	record.timestamp = 0;
	record.date = date_part;
	record.is_valid_date = _isValidDate(record.date);
	if (record.is_valid_date)
	{
		tm	time = _makeDate(record.date);
		record.timestamp = std::mktime(&time);
	}
	record.is_valid_value = _isDouble(number_part);
	record.value = record.is_valid_value ? std::strtod(number_part.c_str(), NULL) : 0;
	return (record);
};

void					BitcoinExchange::_validateRecord(t_record record, bool validate_span)
{
	if (validate_span && record.value < 0) throw std::runtime_error("Error: not a positive number.");
	if (validate_span && record.value > 1000) throw std::runtime_error("Error: too large a number.");
	std::string	error_message = "Error: bad input => ";
	error_message += record.line;
	if (!record.is_valid_input || !record.is_valid_date || !record.is_valid_value) throw std::runtime_error(error_message);
};

tm					BitcoinExchange::_makeDate(const std::string &str)
{
	tm	time;

	std::memset(&time, 0, sizeof(tm));
	time.tm_year = -1;
	time.tm_mon = -1;
	time.tm_mday = -1;
	if (str[4] != '-' || str[7] != '-') return (time);

	std::string	year_str = str.substr(0, 4);
	if (!_isInt(year_str)) return (time);
	time.tm_year = std::atoi(year_str.c_str()) - 1900;

	std::string	month_str = str.substr(5, 2);
	if (!_isInt(month_str)) return (time);
	time.tm_mon = std::atoi(month_str.c_str()) - 1;

	std::string	day_str = str.substr(8, 2);
	if (!_isInt(day_str)) return (time);
	time.tm_mday = std::atoi(day_str.c_str());
	return (time);
};

static bool sorting_records(const t_record& a, const t_record& b)
{
	return a.timestamp < b.timestamp;
}

BitcoinExchange::BitcoinExchange(const std::string &db_name, const std::string &infile_name)
 : _db_name(db_name), _infile_name(infile_name), _db_data(), _infile_data()
{
	if (db_name.empty() || infile_name.empty()) throw std::runtime_error("File Error: 'file name' is empty!");
	_db_data = _readFile(_db_name, ',');
	std::list<t_record>::iterator it = _db_data.begin();
	while (it != _db_data.end())
	{
		_validateRecord(*it, false);
		it++;
	}
	_db_data.sort(sorting_records);
	_infile_data = _readFile(_infile_name, '|');
};

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
 : _db_name(other._db_name), _infile_name(other._infile_name), _db_data(other._db_data), _infile_data(other._infile_data)
{ };

BitcoinExchange::~BitcoinExchange(void)
{

};

BitcoinExchange	&BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this == &other)
		return (*this);
	this->_db_data = other._db_data;
	this->_db_name = other._db_name;
	this->_infile_data = other._infile_data;
	this->_infile_name = other._infile_name;
	return (*this);
};

t_record		BitcoinExchange::_findRecord(t_record src) const
{
	std::list<t_record>::const_iterator	it = _db_data.begin();
	t_record							last_record;
	last_record.timestamp = -1;
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

void			BitcoinExchange::printStatus(void) const
{
	std::list<t_record>::const_iterator	it = _infile_data.begin();
	while (it != _infile_data.end())
	{
		try
		{
			_validateRecord(*it, true);
			t_record	found_record = _findRecord(*it);
			double		result = found_record.value * (*it).value;
			std::cout << (*it).date << " => " << (*it).value << " = " << result << std::endl;
			it++;
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			it++;
		}
	}

};
