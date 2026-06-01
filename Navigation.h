#pragma once
#include "Stack.h"
#include "Navigation.h"
#include "RobotManager.h"

class Layout;

class Navigation
{
public:
	Navigation(Layout& layout);
	~Navigation() {}	

	void moveRobot(Robot* robot, int zone, int aisle, int shelf);
	void getTarget(int zone, int aisle, int shelf, int& targetRow, int& targetCol);

private:
	bool tryMove(Robot* robot, std::string direction);
	void reverseMove(Robot* robot);
	void display();

	Layout& layout;
	Stack* stack;
	std::string* movementLogs;
	int top = -1;
};