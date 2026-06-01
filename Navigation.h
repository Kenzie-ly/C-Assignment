#pragma once
#include "Stack.h"

class Navigation
{
public:
	Navigation();

	void pathToLocation(int zone, int aisle, int shelf);

private:
	Stack* stack = nullptr;
	
};