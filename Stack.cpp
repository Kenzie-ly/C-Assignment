#include "Stack.h"
#include <iostream>

Stack::Stack(int size)
{
	top = -1;
	max = 0;
	createArray(size);
}

void Stack::createArray(int size)
{
	max = size - 1;
	array = new std::string[size];
}

void Stack::push(std::string movement)
{
	if (top == max)
	{
		std::cout << "Stack is full." << '\n';
		return;
	}
	else if (max == 0) std::cout << "Please create an array first" << '\n';

	top++;
	array[top] = movement;
}

std::string Stack::pop()
{
	if (top == -1)
	{
		std::cout << "Stack is empty." << '\n';
		return "";
	}
	std::string mov = array[top];
	array[top] = "";
	top--;
	return mov;
}

std::string Stack::peek()
{
	if (top != -1) return array[top];
	else
	{
		std::cout << "Stack is empty." << '\n';
		return "";
	}
}

bool Stack::isEmpty()
{
	if (top == -1) return true;
	else return false;
}

void Stack::display()
{
	for (int i = top; i >= 0; i--)
	{
		std::cout << array[i] << '\n';
	}
}