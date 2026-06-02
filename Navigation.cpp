#include <iostream>
#include "Navigation.h"
#include "Layout.h"
#include <windows.h>
#include "RobotManager.h"

void Navigation::moveRobot(Robot* robot, int zone, int aisle, int shelf)
{
    for (int i = 0; i <= robot->top; i++) {
        robot->movementLogs[i] = "";
    }
    robot->top = -1;

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
                        reverseMove(robot);
                    }    
        }
        else if (robot->currentCol > targetCol)
        {
            if (!tryMove(robot, "left"))
                if (!tryMove(robot, "up"))
                    if (!tryMove(robot, "down"))
                    {
                        reverseMove(robot);
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
                            reverseMove(robot);
                        }
            }
            else  // go down
            {
                if (!tryMove(robot, "down"))
                    if (!tryMove(robot, "right"))
                        if (!tryMove(robot, "left"))
                        {
                            reverseMove(robot);
                        }
            }
        }
        Sleep(150);
        system("cls");
        layout.display();
        display(robot);
	}

    Sleep(2000);
    while (!robot->stack->isEmpty())
    {
        reverseMove(robot);
        Sleep(150);
        system("cls");
        layout.display();
        display(robot);
        std::cout << '\n' << std::endl;
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
        robot->stack->push(direction);
        robot->top++;
        robot->movementLogs[robot->top] = direction;
        layout.occupyLocation(nextRow, nextCol);
        return true;
    }
    return false;
}

void Navigation::reverseMove(Robot* robot)
{
    layout.deoccupyLocation(robot->currentRow, robot->currentCol);
    std::string direction = robot->stack->pop();
    std::string reversed;
    if (direction == "right")
    {
        robot->currentCol--;
        reversed = "left";
    }
    else if (direction == "left")  
    {
        robot->currentCol++;
        reversed = "right";
    }
    else if (direction == "up")
    {
        robot->currentRow++;
        reversed = "down";
    }
    else if (direction == "down")
    {
        robot->currentRow--;
        reversed = "up";
    }

    robot->top++;
    robot->movementLogs[robot->top] = reversed;
    layout.occupyLocation(robot->currentRow, robot->currentCol);
}

void Navigation::display(Robot* robot)
{
    for (int i = 0; i <= robot->top; i++)
    {
        std::cout << robot->movementLogs[i] << ", ";
    }
}