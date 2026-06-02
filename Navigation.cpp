#include <iostream>
#include "Navigation.hpp"
#include "Layout.hpp"
#include <windows.h>
#include "RobotManager.hpp"

void Navigation::moveRobot(Robot* robot, int zone, int aisle, int shelf)
{
    for (int i = 0; i <= robot->top; i++) {
        robot->movementLogs[i] = "";
    }
    robot->top = -1;

	int targetRow;
	int targetCol;
	getTarget(zone, aisle, shelf, targetRow, targetCol);

    int pathLength = 0;
    std::string* path = findPath(robot->currentRow, robot->currentCol, targetRow, targetCol, pathLength);

    for (int i = 0; i < pathLength; i++) {
        tryMove(robot, path[i]);
        Sleep(150);
        system("cls");
        layout.display();
        display(robot);
    }
    delete[] path;

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

void Navigation::enqueue(PathNode*& head, PathNode*& tail, PathNode* node) {
    node->next = nullptr;
    if (tail) tail->next = node;
    else head = node;
    tail = node;
}

PathNode* Navigation::dequeue(PathNode*& head, PathNode*& tail) {
    if (!head) return nullptr;
    PathNode* node = head;
    head = head->next;
    if (!head) tail = nullptr;
    return node;
}

std::string* Navigation::findPath(int startRow, int startCol, int targetRow, int targetCol, int& pathLength) {
    bool visited[15][45] = {};
    PathNode* head = nullptr;
    PathNode* tail = nullptr;

    PathNode* start = new PathNode();
    start->row = startRow;
    start->col = startCol;
    start->pathLength = 0;
    enqueue(head, tail, start);
    visited[startRow][startCol] = true;

    int dRow[] = { -1, 1, 0, 0 };
    int dCol[] = { 0, 0, -1, 1 };
    std::string dirs[] = { "up", "down", "left", "right" };

    while (head) {
        PathNode* cur = dequeue(head, tail);

        if (cur->row == targetRow && cur->col == targetCol) {
            pathLength = cur->pathLength;
            std::string* result = new std::string[pathLength];
            for (int i = 0; i < pathLength; i++)
                result[i] = cur->path[i];
            delete cur;
            return result;
        }

        for (int i = 0; i < 4; i++) {
            int nr = cur->row + dRow[i];
            int nc = cur->col + dCol[i];
            if (nr < 0 || nr >= 15 || nc < 0 || nc >= 45) continue;
            if (visited[nr][nc] || layout.isObstacle(nr, nc)) continue;

            visited[nr][nc] = true;
            PathNode* next = new PathNode();
            next->row = nr;
            next->col = nc;
            next->pathLength = cur->pathLength + 1;
            for (int j = 0; j < cur->pathLength; j++)
                next->path[j] = cur->path[j];
            next->path[cur->pathLength] = dirs[i];
            enqueue(head, tail, next);
        }
        delete cur;
    }

    pathLength = 0;
    return nullptr;
}