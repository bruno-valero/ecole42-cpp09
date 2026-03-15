/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 20:19:52 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/15 00:05:44 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN(void): _stack(), _support()
{ };

RPN::RPN(const std::string &expression): _stack(), _support()
{
	_parseInput(expression);
};

RPN::RPN(const RPN &other): _stack(other._stack), _support(other._support)
{ };
RPN::~RPN(void)
{ };

RPN					&RPN::operator=(const RPN &other)
{
	if (this == &other)
		return (*this);
	_stack = other._stack;
	_support = other._support;
	return (*this);
};

bool				RPN::_isOperator(const char c) const
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
};

void				RPN::_parseInput(const std::string &input)
{
	if (input.empty()) throw std::runtime_error("Expression Error: empty expression!");
	for (int i = (int)input.length() - 1; i >= 0; i--)
	{
		if (input[i] == ' ') continue ;
		if (std::isdigit(input[i]))
		{
			if ((i == 0 && std::isdigit(input[i + 1]))
				|| (i == (int)input.length() - 1 && std::isdigit(input[i - 1]))
				|| (i > 0 && (std::isdigit(input[i - 1]) || std::isdigit(input[i + 1])))
			)
			{
				while (!_stack.empty())
					_stack.pop();
				throw std::runtime_error("Expression Error: number greater than '9'!");
			}
			t_item	item = t_item();
			item.type = NUMBER;
			item.number = input[i] - '0';
			_stack.push(item);
			continue ;
		}
		if (_isOperator(input[i]))
		{
			t_item	item = t_item();
			item.type = OPERATOR;
			item.operat = input[i];
			_stack.push(item);
			continue ;
		}
		std::string	error_message = "Expression Error: wrong character: '";
		error_message += input[i];
		error_message += "'";
		throw std::runtime_error(error_message.c_str());
	}
};

t_item				RPN::_calc(const double nbr1, const double nbr2, const char operat)
{
	t_item	item = t_item();
	item.type = NUMBER;
	switch (operat)
	{
	case '+':
		item.number = nbr1 + nbr2; break;
	case '-':
		item.number = nbr1 - nbr2; break;
	case '*':
		item.number = nbr1 * nbr2; break;
	case '/':
		item.number = nbr1 / nbr2; break;

	default:
		break;
	}
	return (item);
};

void				RPN::_supportToStack(void)
{
	while (!_support.empty())
	{
		_stack.push(_support.top());
		_support.pop();
	}
};

void				RPN::setExpression(const std::string &input)
{
	while (!_stack.empty())
		_stack.pop();
	while (!_support.empty())
		_support.pop();
	_parseInput(input);
};

double			RPN::claculateExpression(void)
{
	static bool	start_calculate = false;
	if (_stack.top().type == OPERATOR) throw std::runtime_error("Expression Error: operator at start!");
	while (!_stack.empty())
	{
		t_item item = _stack.top();
		while (item.type != OPERATOR)
		{
			_support.push(item);
			_stack.pop();
			item = _stack.top();
		}
		if (_stack.empty()) throw std::runtime_error("Expression Error: not enough operators!");
		if (!start_calculate && _support.size() == 1) throw std::runtime_error("Expression Error: not enough numbers!");
		if (start_calculate && _support.size() == 1 && _stack.size() > 0) throw std::runtime_error("Expression Error: not enough numbers!");
		if (start_calculate && _support.size() == 1) return (_support.top().number);
		t_item	nbr1 = _support.top();
		_support.pop();
		t_item	nbr2 = _support.top();
		_support.pop();
		_support.push(_calc(nbr2.number, nbr1.number, _stack.top().operat));
		_stack.pop();
		start_calculate = true;
	}
	if (_support.size() > 1) throw std::runtime_error("Expression Error: not enough operators!");
	return (_support.top().number);
};

