// who: Jiahao Liang
// why: Project 2
// when: 11-09-2021
/**
***	For extra credit:
***		My program is to be able to handle illegal expressions and can handle optional spacing between the two tokens. 
		An illegal expression may contain tokens that are neither integers nor valid operators nor parenthesis. 
***
**/

#pragma once
#ifndef _CALCULATOR_H
#define _CALCULATOR_H

#include "sq_stack.h"

#include <cctype>
#include <climits>
#include <cmath>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

template <class T, int N>
class calculator
{
public:
	calculator(const std::string&);
	~calculator();
	std::string postfix() const noexcept;		 // returns the postfix expression.
	T eval() noexcept(false);					 // returns the result of the expression.
	inline void setExpr(const std::string&);	 // receives the infix expression as string and store it.
	inline std::string getExpr() const noexcept; // returns the infix expression as string

private:
	inline T additiveOperation(const T&, const T&);
	inline T subtractiveOperations(const T&, const T&);
	inline T multiplicationOperations(const T&, const T&);
	inline T divisionOperation(const T&, const T&);
	inline T squareOperation(const T&, const T&);
	inline T moduloOperation(const T&, const T &);
	inline T factorialOperation(T);
	void infixToPostfix() noexcept(false);
	inline bool pushNumber(const std::string::iterator&);
	inline void pushSymbol(const char&);
	inline void isNumberOverflowing(const T&) const noexcept(false);
	void isIllegalParenthesis() const noexcept(false);
	void isIllegalExpression() noexcept(false);
	void eraseSpaces();
	inline int optrPrecedence(const char&) const noexcept;

private:
	std::unique_ptr<SqStack<char, N>> operators;
	std::unique_ptr<SqStack<T, N>> operands;
	std::string infixExpression;
	std::string postfixExpression;
};

template <class T, int N>
calculator<T, N>::calculator(const std::string &expression) : operators(std::make_unique<SqStack<char, N>>()), operands(std::make_unique<SqStack<T, N>>()), infixExpression(expression), postfixExpression("")
{
	infixToPostfix();
}

template <class T, int N>
calculator<T, N>::~calculator()
{
	operators.reset();
	operators = nullptr;
	operands.reset();
	operands = nullptr;
}

template <class T, int N>
T calculator<T, N>::eval() noexcept(false)
{
	T result = 1.0;
	T tempOperand = 0.0;
	bool isNegativeNum = false;
	for (auto i = postfixExpression.begin(); i != postfixExpression.end(); ++i)
	{
		if (std::isdigit(*i))
		{
			std::stringstream sstr;
			sstr << *i;
			while (std::isdigit(*(i + 1)))
			{
				++i;
				sstr << *i;
			}
			sstr >> tempOperand;
			if(isNegativeNum)
			{
				tempOperand *= -1;
				isNegativeNum = false;
			}
			isNumberOverflowing(tempOperand);
			operands->push(tempOperand);
			continue;
		}
		else if(*i == 'N')
		{
			isNegativeNum = true;
			continue;
		}
		else if(*i == ' ')
		{
			continue;
		}
		tempOperand = operands->peek();
		operands->pop();
		switch (*i)
		{
		case '+':
			result = additiveOperation(operands->peek(), tempOperand);
			break;
		case '-':
			result = subtractiveOperations(operands->peek(), tempOperand);
			break;
		case '*':
			result = multiplicationOperations(operands->peek(), tempOperand);
			break;
		case '/':
			result = divisionOperation(operands->peek(), tempOperand);
			break;
		case '^':
			result = squareOperation(operands->peek(), tempOperand);
			break;
		case '%':
			result = (static_cast<int>(operands->peek()) % static_cast<int>(tempOperand));
			operands->pop();
			break;
		case '!':
			result = factorialOperation(tempOperand);
			break;
		}
		operands->push(result);
	}
	return operands->peek();
}

template <class T, int N>
inline T calculator<T, N>::additiveOperation(const T &summand, const T &addend)
{
	isNumberOverflowing(summand + addend);
	T result = (summand + addend);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::subtractiveOperations(const T &minute, const T &minus)
{
	isNumberOverflowing(minute - minus);
	T result = (minute - minus);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::multiplicationOperations(const T &multiplicand, const T &multiplier)
{
	isNumberOverflowing(multiplicand * multiplier);
	T result = (multiplicand * multiplier);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::divisionOperation(const T &dividend, const T &divisor)
{
	isNumberOverflowing(dividend / divisor);
	T result = (dividend / divisor);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::squareOperation(const T &num, const T &Squared)
{
	isNumberOverflowing(std::pow(num, Squared));
	T result = std::pow(num, Squared);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::moduloOperation(const T &num1, const T &num2)
{
	isNumberOverflowing(num1 % num2);
	T result = (num1 % num2);
	operands->pop();
	return result;
}

template <class T, int N>
inline T calculator<T, N>::factorialOperation(T num)
{
	T result = 1;
	while (num > 1)
	{
		result *= num;
		--num;
	}
	isNumberOverflowing(result);
	return result;
}

template <class T, int N>
std::string calculator<T, N>::postfix() const noexcept
{
	std::string str = postfixExpression;
	for(auto i = str.begin(); i != str.end(); ++i)
	{
		if(*i == 'N')
		{
			*i = '-';
			str.erase(i+1);
		}
	}
	return str;
}

template <class T, int N>
inline void calculator<T, N>::setExpr(const std::string &infixExpression)
{
	this->infixExpression = infixExpression;
	infixToPostfix();
}

template <class T, int N>
inline std::string calculator<T, N>::getExpr() const noexcept
{
	return infixExpression;
}

template <class T, int N>
void calculator<T, N>::infixToPostfix() noexcept(false)
{
	eraseSpaces();
	isIllegalExpression();
	isIllegalParenthesis();
	for (auto i = infixExpression.begin(); i != infixExpression.end(); ++i)
	{
		if(pushNumber(i))
		{
			continue;
		}
		switch (*i)
		{
		case '+':
		case '-':
		case '*':
		case '/':
		case '%':
		case '!':
			pushSymbol(*i);
			break;
		case '^':
			if(optrPrecedence(*i) == optrPrecedence(operators->peek()) || operators->isEmpty())
			{
				operators->push(*i);
			}
			else
			{
				pushSymbol(*i);
			}
			break;
		case '(':
			operators->push(*i);
			break;
		case ')':
			while (operators->peek() != '(')
			{
				postfixExpression += operators->peek();
				postfixExpression += " ";
				operators->pop();
			}
			operators->pop();
			break;
		case 'N':
			postfixExpression += *i;
			postfixExpression += " ";
			break;
		default:
			throw std::invalid_argument("ERROR: INVALID CHARACTER");
		}
	}
	while (!operators->isEmpty())
	{
		postfixExpression += operators->peek();
		postfixExpression += " ";
		operators->pop();
	}
}

template <class T, int N>
inline bool calculator<T, N>::pushNumber(const std::string::iterator &i)
{
	if (std::isdigit(*i))
	{
		postfixExpression += *i;
		if (!std::isdigit(*(i+1)))
		{
			postfixExpression += " ";
		}
		return true;
	}
	return false;
}

template <class T, int N>
inline void calculator<T, N>::pushSymbol(const char &ch)
{
	if(optrPrecedence(ch) > optrPrecedence(operators->peek()) || operators->isEmpty())
	{
		operators->push(ch);
	}
	else
	{
		postfixExpression += operators->peek();
		postfixExpression += " ";
		operators->pop();
		operators->push(ch);
	}
}

template <class T, int N>
inline void calculator<T, N>::isNumberOverflowing(const T &num) const noexcept(false)
{
	if (num * 10 / 10 != num)
	{
		throw std::overflow_error("ERROR: DIGITAL OVERFLOWS");
	}
}

template <class T, int N>
void calculator<T, N>::isIllegalExpression() noexcept(false)
{
	for (auto i = infixExpression.begin(); i != infixExpression.end(); ++i)
	{
		switch (*i)
		{
		case '+':
		case '*':
		case '/':
		case '%':
		case '^':
		case '!':
			if (*i == *(i + 1))
			{
				throw std::invalid_argument("ERROR: EXPECTED EXPRESSION");
			}
			else if (*(i + 1) == '-')
			{
				*(i + 1) = 'N';
				++i;
			}
			break;
		case '-':
			if(*i == infixExpression.front())
			{
				*i = 'N';
			}
			if ('-' == *(i + 1))
			{
				*i = '+';
				infixExpression.erase(i + 1);
				++i;
			}
			break;
		}
	}
}

template <class T, int N>
void calculator<T, N>::isIllegalParenthesis() const noexcept(false)
{
	size_t rightParenthesesCount = 0;
	size_t leftParenthesesCount = 0;
	for (auto i = infixExpression.begin(); i != infixExpression.end(); i++)
	{
		if (*i == '(')
		{
			leftParenthesesCount++;
		}
		else if (*i == ')')
		{
			rightParenthesesCount++;
		}
	}
	if (leftParenthesesCount != rightParenthesesCount)
	{
		throw std::invalid_argument("ERROR: EXPECTED EXPRESSION");
	}
}

template <class T, int N>
inline void calculator<T, N>::eraseSpaces()
{
	for (auto i = infixExpression.begin(); i != infixExpression.end(); ++i)
	{
		if (*i == ' ')
		{
			infixExpression.erase(i);
		}
	}
}

template <class T, int N>
inline int calculator<T, N>::optrPrecedence(const char &optr) const noexcept
{
	switch (optr)
	{
	case '+':
	case '-':
		return 1;
	case '*':
	case '/':
	case '%':
		return 2;
	case '^':
		return 3;
	case '!':
		return 4;
	default:
		return -1;
	}
}

#endif