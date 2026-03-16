/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/15 10:30:17 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/15 22:48:03 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
# define PMERGEME_HPP

# include <iostream>
# include <vector>
# include <deque>
# include <utility>
# include <algorithm>
# include <string>
# include <cstring>
# include <stdexcept>
# include <limits>
# include <sys/time.h>
# include <iomanip>

class PmergeMe
{
private:
	std::vector<int>			_vector;
	std::deque<int>				_deque;
	std::vector<int>			_ordered_vector;
	std::deque<int>				_ordered_deque;

	static time_t				_timer_begin;
	static time_t				_timer_end;
	static time_t				_process_vector_time;
	static time_t				_process_deque_time;

	void						_addToVector(int value);
	void						_addToDeque(int value);
	void						_parseInput(const char **data, size_t size, void (PmergeMe::*addToContainer)(int));
	void						_processVector(const char **data, size_t size);
	void						_processDeque(const char **data, size_t size);
	void						_setVector(const char **data, size_t size);
	void						_setDeque(const char **data, size_t size);

	static void					_isSorted(const std::vector<int> &data);
	static void					_isSorted(const std::deque<int> &data);

	static std::vector<size_t>	_jacobsthalOrder(const std::vector<std::pair<int, int> > &data);
	static std::deque<size_t>	_jacobsthalOrder(const std::deque<std::pair<int, int> > &data);
	static size_t				_binarySearch(const std::vector<int> &data, int limit, int value);
	static size_t				_binarySearch(const std::deque<int> &data, int limit, int value);
	static std::vector<int>		_fordJohnson(const std::vector<int> &data);
	static std::deque<int>		_fordJohnson(const std::deque<int> &data);

	static void					_startTimer(void);
	static void					_stopTimer(void);
	static time_t				_getTimeInterval(void);

	static void					_printContainer(const std::vector<int> &data);
	static void					_printContainer(const std::deque<int> &data);
	static void					_printProcessTime(const std::vector<int> &data);
	static void					_printProcessTime(const std::deque<int> &data);

public:
	PmergeMe(void);
	PmergeMe(const char **data, size_t size);
	PmergeMe(const PmergeMe &other);
	~PmergeMe(void);
	PmergeMe					&operator=(const PmergeMe &other);


	void						processContainers(const char **data, size_t size);
	const std::vector<int>		&getVector(void) const;
	const std::deque<int>		&getDeque(void) const;





};

#endif
