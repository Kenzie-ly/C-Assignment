#include <iostream>
#include "Navigation.h"

Navigation::Navigation()
{
	stack = new Stack();
}

void Navigation::pathToLocation(int zone, int aisle, int shelf)
{
	std::cout << "Pathing to Zone " << zone << ", Aisle " << aisle << ", Shelf " << shelf << '\n';

	if (zone == 1 || zone == 2)
	{
		stack->push("forward");
	}
	else if (zone == 3 || zone == 4)
	{
		stack->push("right");
	}

}