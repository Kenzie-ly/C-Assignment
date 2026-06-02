#pragma once
#include "Stack.h"
#include "Navigation.h"
#include "RobotManager.h"

class Layout;
class Robot;

struct PathNode {
	int row;
	int col;
	std::string path[200]; // directions so far
	int pathLength;
	PathNode* next;
};

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
	void enqueue(PathNode*& head, PathNode*& tail, PathNode* node);
	PathNode* dequeue(PathNode*& head, PathNode*& tail);
	std::string* findPath(int startRow, int startCol, int targetRow, int targetCol, int& pathLength);

	Layout& layout;
};