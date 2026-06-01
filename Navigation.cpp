#include <iostream>
#include "Navigation.h"
#include "Layout.h"
#include <windows.h>

Navigation::Navigation(Layout& layout) : layout(layout)
{
	stack = new Stack(200);
	
}

void Navigation::moveRobot(Robot* robot, int zone, int aisle, int shelf)
{
	int targetRow;
	int targetCol;
	getTarget(zone, aisle, shelf, targetRow, targetCol);

	while (robot->currentRow != targetRow || robot->currentCol != targetCol)
	{
        if (robot->currentCol < targetCol)
        {
            if (!tryMove(robot, "right"))
                if (!tryMove(robot, "up"))
                    if (!tryMove(robot, "down"))
                    {
                        std::string lastMove = stack->pop();
                        reverseMove(robot, lastMove);
                    }    
        }
        else if (robot->currentCol > targetCol)
        {
            if (!tryMove(robot, "left"))
                if (!tryMove(robot, "up"))
                    if (!tryMove(robot, "down"))
                    {
                        std::string lastMove = stack->pop();
                        reverseMove(robot, lastMove);
                    }
        }
        else
        {
            if (robot->currentRow > targetRow)  // go up
            {
                if (!tryMove(robot, "up"))
                    if (!tryMove(robot, "right"))
                        if (!tryMove(robot, "left"))
                        {
                            std::string lastMove = stack->pop();
                            reverseMove(robot, lastMove);
                        }
            }
            else  // go down
            {
                if (!tryMove(robot, "down"))
                    if (!tryMove(robot, "right"))
                        if (!tryMove(robot, "left"))
                        {
                            std::string lastMove = stack->pop();
                            reverseMove(robot, lastMove);
                        }
            }
        }
        layout.display();
        Sleep(1000);
	}
}

void Navigation::getTarget(int zone, int aisle, int shelf, int& targetRow, int& targetCol)
{
	int* cols = layout.getZoneCol(zone);
	int* rows = layout.getZoneRow(zone);
	targetCol = cols[(aisle - 1) * 2 + ((shelf-1) % 2)];
	targetRow = rows[1];
}

bool Navigation::tryMove(Robot* robot, std::string direction)
{
    int nextRow = robot->currentRow;
    int nextCol = robot->currentCol;

    if (direction == "right") nextCol++;
    else if (direction == "left") nextCol--;
    else if (direction == "up") nextRow--;
    else if (direction == "down") nextRow++;

    if (nextRow < 0 || nextRow >= 15 || nextCol < 0 || nextCol >= 45)
        return false;

    if (!layout.isObstacle(nextRow, nextCol))
    {
        layout.deoccupyLocation(robot->currentRow, robot->currentCol);
        robot->currentRow = nextRow;
        robot->currentCol = nextCol;
        stack->push(direction);
        layout.occupyLocation(nextRow, nextCol);
        return true;
    }
    return false;
}

void Navigation::reverseMove(Robot* robot, std::string direction)
{
    if (direction == "right") robot->currentCol--;
    if (direction == "left")  robot->currentCol++;
    if (direction == "up")    robot->currentRow++;
    if (direction == "down")  robot->currentRow--;
}