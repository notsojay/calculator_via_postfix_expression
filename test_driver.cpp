// who: Jiahao Liang
// why: Project 2
// when: 11-09-2021
/**
***	For extra credit:
***		My program is to be able to handle illegal expressions and can handle optional spacing between the two tokens. 
		An illegal expression may contain tokens that are neither integers nor valid operators nor parenthesis. 
***
**/

#include "calculator.h"

#include <iomanip>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#define WINDOWS false
const size_t expressionLength = 999;

inline void printGreetingMessage();
inline std::string getExpression();
void printResult(const std::unique_ptr<calculator<double, expressionLength>> &);
template <typename T>
inline T getUserInput(const std::string &prompt);
inline void clearScreen();
inline void clearInstream();

int main(int argc, char *argv[])
{
	printGreetingMessage();
	do
	{
		std::string expression = getExpression();
		if (expression == "0")
		{
			break;
		}
		try
		{
			std::unique_ptr<calculator<double, expressionLength>> calc = std::make_unique<calculator<double, expressionLength>>(expression);
			printResult(calc);
			calc.reset();
		}
		catch (std::overflow_error &oe)
		{
			clearInstream();
			std::cout << oe.what() << std::endl;
			continue;
		}
		catch (std::underflow_error &ue)
		{
			clearInstream();
			std::cout << ue.what() << std::endl;
			continue;
		}
		catch (std::out_of_range &oor)
		{
			clearInstream();
			std::cout << oor.what() << std::endl;
			continue;
		}
		catch (std::invalid_argument &ia)
		{
			clearInstream();
			std::cout << ia.what() << std::endl;
			continue;
		}
		clearInstream();
	} while (1);
	std::cout << "Thanks for using my calculator." << std::endl;
	return 0;
}

inline void printGreetingMessage()
{
	std::cout << "[Jiahao Liang} Calculator Enter 0 to exit the program." << std::endl;
}

inline std::string getExpression()
{
	return getUserInput<std::string>("Enter an expression: ");
}

void printResult(const std::unique_ptr<calculator<double, expressionLength>> &calc)
{
	std::cout << "Postfix expression: " << calc->postfix() << std::endl;
	double result = calc->eval();
	std::cout << std::fixed << std::setprecision(2) << "Result: " << result << std::endl;
}

template <typename T>
inline T getUserInput(const std::string &prompt)
{
	T input;
	std::cout << prompt;
	getline(std::cin, input);
	return input;
}

inline void clearScreen()
{
	if (WINDOWS)
	{
		system("CLS");
	}
	else
	{
		system("clear");
	}
}

inline void clearInstream()
{
	std::cin.sync();
}