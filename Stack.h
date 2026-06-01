#pragma once
#include <string>

class Stack
{
public:
	Stack(int size);
	~Stack() { delete[] array; }

	void createArray(int size);
	void push(std::string movement);
	std::string pop();
	std::string peek();
	bool isEmpty();
	void display();
	void displayReverse();

private:
	std::string* array = nullptr;
	int top, max;
};