/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 11:19:02 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/15 23:11:06 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

time_t PmergeMe::_timer_begin = 0;
time_t PmergeMe::_timer_end = 0;
time_t PmergeMe::_process_vector_time = 0;
time_t PmergeMe::_process_deque_time = 0;

PmergeMe::PmergeMe(void): _vector(), _deque(), _ordered_vector(), _ordered_deque()
{ };

PmergeMe::PmergeMe(const char **data, size_t size)
 : _vector(), _deque(), _ordered_vector(), _ordered_deque()
{
	processContainers(data, size);
};

PmergeMe::PmergeMe(const PmergeMe &other)
 : _vector(other._vector), _deque(other._deque), _ordered_vector(other._ordered_vector), _ordered_deque(other._ordered_deque)
{ };

PmergeMe::~PmergeMe(void)
{ };

PmergeMe					&PmergeMe::operator=(const PmergeMe &other)
{
	if (this == &other)
		return (*this);
	_vector = other._vector;
	_deque = other._deque;
	_ordered_vector = other._ordered_vector;
	_ordered_deque = other._ordered_deque;
	return (*this);
};

const std::vector<int>		&PmergeMe::getVector(void) const
{
	return (_ordered_vector);
};

const std::deque<int>		&PmergeMe::getDeque(void) const
{
	return (_ordered_deque);
};

void						PmergeMe::_addToVector(int value)
{
	_vector.push_back(value);
};

void						PmergeMe::_addToDeque(int value)
{
	_deque.push_back(value);
};

void						PmergeMe::_parseInput(const char **data, size_t size, void (PmergeMe::*addToContainer)(int))
{
	for (size_t i = 0; i < size; i++)
	{
		size_t		number_len = 0;
		bool	found_plus = false;
		for(int j = 0; data[i][j]; j++)
		{
			if (std::isspace(data[i][j]))
				continue ;
			if (!found_plus && data[i][j] == '+')
			{
				found_plus = true;
				continue ;
			}
			if (!std::isdigit(data[i][j]))
			{
				std::string	error_message = "Input Error: character not allowed '";
				error_message += data[i][j];
				error_message += "'.";
				throw std::runtime_error(error_message);
			}
			number_len++;
		}

		long long	number = std::atoll(data[i]);
		if (number < 0 || number > std::numeric_limits<int>::max() || number_len > 7)
		{
			std::string	error_message = "Input Error: number out of bound '";
			error_message += data[i];
			error_message += "'! It must be between '0' and '";
			error_message += std::numeric_limits<int>::max();
			error_message += "'.";
			throw std::runtime_error(error_message);
		}
		(this->*addToContainer)(static_cast<int>(number));
	}
};

void						PmergeMe::_processVector(const char **data, size_t size)
{
	_startTimer();
	_setVector(data, size);
	_ordered_vector = _fordJohnson(_vector);
	_isSorted(_ordered_vector);
	_stopTimer();
};

void						PmergeMe::_processDeque(const char **data, size_t size)
{
	_startTimer();
	_setDeque(data, size);
	_ordered_deque = _fordJohnson(_deque);
	_isSorted(_ordered_deque);
	_stopTimer();
};

void						PmergeMe::processContainers(const char **data, size_t size)
{
	_processVector(data, size);
	_process_vector_time = _getTimeInterval();
	_processDeque(data, size);
	_process_deque_time = _getTimeInterval();

	std::cout << "Before: " << std::setw(10);
	_printContainer(_vector);

	std::cout << "After: " << std::setw(10);
	_printContainer(_ordered_vector);

	_printProcessTime(_ordered_vector);
	_printProcessTime(_ordered_deque);
};

void						PmergeMe::_setVector(const char **data, size_t size)
{
	if (!_vector.empty())
		_vector.clear();
	_parseInput(data, size, &PmergeMe::_addToVector);
};

void						PmergeMe::_setDeque(const char **data, size_t size)
{
	if (!_deque.empty())
		_deque.clear();
	_parseInput(data, size, &PmergeMe::_addToDeque);
};

void					PmergeMe::_isSorted(const std::vector<int> &data)
{
	int	last_integer = -1;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] < last_integer) throw std::runtime_error("Sorting Error: 'vector' is not sorted!");
		last_integer = data[i];
	}
};

void					PmergeMe::_isSorted(const std::deque<int> &data)
{
	int	last_integer = -1;
	for (size_t i = 0; i < data.size(); i++)
	{
		if (data[i] < last_integer) throw std::runtime_error("Sorting Error: 'deque' is not sorted!");
		last_integer = data[i];
	}
};

std::vector<size_t>		PmergeMe::_jacobsthalOrder(const std::vector<std::pair<int, int> > &data)
{
	size_t	n = data.size();
	std::vector<size_t> order;
	if (n == 0) return (order);
	order.push_back(0);

	size_t j1 = 1, j2 = 1;
	while (order.size() < n)
	{
		size_t j = j1 + 2 * (order.size() >= 2 ? j2 : 0);
		if (j < n)
			order.push_back(j);
		else
			break ;
		j2 = j1;
		j1 = j;
	}

	std::vector<bool> used(n, false);
	for (size_t i = 0; i < order.size(); i++) used[order[i]] = true;

	for (size_t i = 0; i < n; i++)
		if (!used[i])
			order.push_back(i);

	return (order);
};

std::deque<size_t>		PmergeMe::_jacobsthalOrder(const std::deque<std::pair<int, int> > &data)
{
	size_t	n = data.size();
	std::deque<size_t> order;
	if (n == 0) return (order);
	order.push_back(0);

	size_t j1 = 1, j2 = 1;
	while (order.size() < n)
	{
		size_t j = j1 + 2 * (order.size() >= 2 ? j2 : 0);
		if (j < n)
			order.push_back(j);
		else
			break ;
		j2 = j1;
		j1 = j;
	}

	std::deque<bool> used(n, false);
	for (size_t i = 0; i < order.size(); i++) used[order[i]] = true;

	for (size_t i = 0; i < n; i++)
		if (!used[i])
			order.push_back(i);

	return (order);
};

size_t					PmergeMe::_binarySearch(const std::vector<int> &data, int limit, int value)
{
	if (value > data[limit])
		return (limit + 1);
	long long	begin = 0;
	long long	end = limit;
	long long	mid;
	while (begin < end)
	{
		mid = (begin + end) / 2;
		if (value == data[mid])
			return (mid);
		if (value > data[mid])
			begin = mid + 1;
		else
			end = mid;
	}
	return (end);
};

size_t					PmergeMe::_binarySearch(const std::deque<int> &data, int limit, int value)
{
	if (value > data[limit])
		return (limit + 1);
	long long	begin = 0;
	long long	end = limit;
	long long	mid;
	while (begin < end)
	{
		mid = (begin + end) / 2;
		if (value == data[mid])
			return (mid);
		if (value > data[mid])
			begin = mid + 1;
		else
			end = mid;
	}
	return (end);
};

std::vector<int>		PmergeMe::_fordJohnson(const std::vector<int> &data)
{
	std::vector<std::pair<int, int> >	paired;
	std::vector<int>					greater_values;
	int									remain = -1;

	if (data.size() <= 1)
		return (data);
	if (data.size() == 2)
	{
		greater_values = data;
		if (greater_values[0] > greater_values[1])
			std::swap(greater_values[0], greater_values[1]);
		return (greater_values);
	}

	for (size_t i = 0; i < data.size(); i += 2)
	{
		if (i + 1 == data.size())
		{
			remain = data[i];
			break ;
		}
		if (data[i] > data[i + 1])
			paired.push_back(std::make_pair(data[i + 1], data[i]));
		else
			paired.push_back(std::make_pair(data[i], data[i + 1]));
		greater_values.push_back(paired.back().second);
	}
	greater_values = _fordJohnson(greater_values);
	size_t	inser_idx;
	size_t	limit_idx;
	std::vector<size_t>	order = _jacobsthalOrder(paired);
	for (size_t i = 0; i < order.size(); i++)
	{
		limit_idx = _binarySearch(greater_values, greater_values.size() - 1, paired[order[i]].second);
		inser_idx = _binarySearch(greater_values, limit_idx, paired[order[i]].first);
		greater_values.insert(greater_values.begin() + inser_idx, paired[order[i]].first);
	}
	if (remain != -1)
	{
		inser_idx = _binarySearch(greater_values, greater_values.size() - 1, remain);
		greater_values.insert(greater_values.begin() + inser_idx, remain);
	}
	return (greater_values);
};

std::deque<int>		PmergeMe::_fordJohnson(const std::deque<int> &data)
{
	std::deque<std::pair<int, int> >	paired;
	std::deque<int>						greater_values;
	int									remain = -1;

	if (data.size() <= 1)
		return (data);
	if (data.size() == 2)
	{
		greater_values = data;
		if (greater_values[0] > greater_values[1])
			std::swap(greater_values[0], greater_values[1]);
		return (greater_values);
	}

	for (size_t i = 0; i < data.size(); i += 2)
	{
		if (i + 1 == data.size())
		{
			remain = data[i];
			break ;
		}
		if (data[i] > data[i + 1])
			paired.push_back(std::make_pair(data[i + 1], data[i]));
		else
			paired.push_back(std::make_pair(data[i], data[i + 1]));
		greater_values.push_back(paired.back().second);
	}
	greater_values = _fordJohnson(greater_values);
	size_t	inser_idx;
	size_t	limit_idx;
	std::deque<size_t>	order = _jacobsthalOrder(paired);
	for (size_t i = 0; i < order.size(); i++)
	{
		limit_idx = _binarySearch(greater_values, greater_values.size() - 1, paired[order[i]].second);
		inser_idx = _binarySearch(greater_values, limit_idx, paired[order[i]].first);
		greater_values.insert(greater_values.begin() + inser_idx, paired[order[i]].first);
	}
	if (remain != -1)
	{
		inser_idx = _binarySearch(greater_values, greater_values.size() - 1, remain);
		greater_values.insert(greater_values.begin() + inser_idx, remain);
	}
	return (greater_values);
};

void					PmergeMe::_startTimer(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	_timer_begin = tv.tv_sec * 1000000LL + tv.tv_usec;
};

void					PmergeMe::_stopTimer(void)
{
	struct timeval	tv;
	gettimeofday(&tv, NULL);
	_timer_end = tv.tv_sec * 1000000LL + tv.tv_usec;
};

time_t				PmergeMe::_getTimeInterval(void)
{
	return (_timer_end - _timer_begin);
};

void					PmergeMe::_printContainer(const std::vector<int> &data)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		std::cout << data[i];
		if (i < data.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
};

void					PmergeMe::_printContainer(const std::deque<int> &data)
{
	for (size_t i = 0; i < data.size(); i++)
	{
		std::cout << data[i];
		if (i < data.size() - 1)
			std::cout << " ";
	}
	std::cout << std::endl;
};

void					PmergeMe::_printProcessTime(const std::vector<int> &data)
{
	std::cout
		<< "Time to process a range of " << std::setw(10)
		<< data.size()
		<< " elements with std::vector : " << std::setw(10)
		<< std::fixed << std::setprecision(5)
		<< ((double)_process_vector_time / 1000.0) << " us" << std::endl;
};

void					PmergeMe::_printProcessTime(const std::deque<int> &data)
{
	std::cout
		<< "Time to process a range of " << std::setw(10)
		<< data.size()
		<< " elements with std::deque : " << std::setw(11)
		<< std::fixed << std::setprecision(5)
		<< ((double)_process_deque_time / 1000.0) << " us" << std::endl;
};
