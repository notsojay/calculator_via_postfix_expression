//
// sq_stack.h
// Created by Jiahao Liang on 10/27/21.

#pragma once
#ifndef _SQ_STACK_H_
#define _SQ_STACK_H_

#include <memory>
#include <array>
#include <stdexcept>
#include <iostream>

template<class T, int N>
class SqStack
{
public:
	SqStack();
	~SqStack();
	inline bool isEmpty() const noexcept;
	inline bool isFull() const noexcept;
	inline void push(const T&) noexcept(false);
	inline void pop() noexcept(false);
	inline void replace(const T&, const int&) noexcept(false);
	inline T peek() const;
	inline int size() const noexcept;
	friend std::ostream& operator<<(std::ostream &out, const SqStack &other)
	{
		for(const auto &i : *(other.stackArray))
		{
			out << i << " ";
		}
		return out;
	}
	
private:
	std::unique_ptr<std::array<T, N>> stackArray;
	int top;
};

template<class T, int N>
SqStack<T, N>::SqStack():stackArray(std::make_unique<std::array<T,N>>(std::array<T,N>{})), top(-1){}

template<class T, int N>
SqStack<T, N>::~SqStack()
{
	stackArray.reset();
	stackArray = nullptr;
}

template<class T, int N>
inline bool SqStack<T, N>::isEmpty() const noexcept
{
	return top == -1;
}

template<class T, int N>
inline bool SqStack<T, N>::isFull() const noexcept
{
	return top >= N-1;
}

template<class T, int N>
inline void SqStack<T, N>::push(const T &data) noexcept(false)
{
	if(isFull())
	{
		throw std::out_of_range("ERROR: OUT OF RANGE");
	}
	++top;
	stackArray->at(top) = data;
}

template<class T, int N>
inline void SqStack<T, N>::pop() noexcept(false)
{
	if(isEmpty())
	{
		throw std::out_of_range("ERROR: OUT OF RANGE");
	}
	--top;
}

template<class T, int N>
inline void SqStack<T, N>::replace(const T &data, const int &index) noexcept(false)
{
	if(isEmpty() || index < 0)
	{
		throw std::out_of_range("ERROR: OUT OF RANGE");
	}
	if(index >= N-1)
	{
		throw std::out_of_range("ERROR: OUT OF RANGE");
	}
	stackArray->at(index) = data;
}

template<class T, int N>
inline T SqStack<T, N>::peek() const
{
	if(isEmpty())
	{
		return -1;
	}
	return stackArray->at(top);
}

template<class T, int N>
inline int SqStack<T, N>::size() const noexcept
{
	return top == -1 ? 0 : top+2;
}

#endif