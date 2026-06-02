#pragma once
#include "Stack.h"
#include "Navigation.h"
#include "RobotManager.h"

class Layout;
class Robot;

class Navigation
{
public:
	Navigation(Layout& layout) : layout(layout) {};
	~Navigation() {}	

	void moveRobot(Robot* robot, int zone, int aisle, int shelf);
	void getTarget(int zone, int aisle, int shelf, int& targetRow, int& targetCol);

private:
	bool tryMove(Robot* robot, std::string direction);
	void reverseMove(Robot* robot);
	void display(Robot* robot);

	Layout& layout;
	int top = -1;
};