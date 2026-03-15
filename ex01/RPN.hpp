/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bruno-valero <bruno-valero@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 19:56:22 by bruno-valer       #+#    #+#             */
/*   Updated: 2026/03/15 00:04:31 by bruno-valer      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
# define RPN_HPP

# include <string>
# include <stack>
# include <stdexcept>
# include <cstdlib>

typedef enum e_item_type
{
	OPERATOR,
	NUMBER
}	t_item_type;

typedef struct s_item		t_item;
struct s_item
{
	t_item_type	type;
	double		number;
	char		operat;
};

class RPN
{
	private:
		static const int	_max_expression_number = 9;

		std::stack<t_item>	_stack;
		std::stack<t_item>	_support;

		bool				_isOperator(const char c) const;
		void				_parseInput(const std::string &input);
		t_item				_calc(const double nbr1, const double nbr2, const char operat);
		/**
		 * ## <private> _supportToStack
		 *
		 * Put every item of `_support` on `_stack`.
		 */
		void				_supportToStack(void);

	public:
		RPN(void);
		RPN(const std::string &expression);
		RPN(const RPN &other);
		~RPN(void);

		RPN					&operator=(const RPN &other);

		void				setExpression(const std::string &input);
		double				claculateExpression(void);
};

#endif
