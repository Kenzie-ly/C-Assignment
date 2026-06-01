#pragma once

class Layout
{
public:
	Layout();
    ~Layout() {}

    enum Cell {
        WALKABLE = 0,
        OBSTACLE = 1,
        ZONE_A = 2,
        ZONE_B = 3,
        ZONE_C = 4,
        ZONE_D = 5,
        SHELF = 6,
        AISLE = 7,
        OCCUPIED = 8
    };

    void display();
    void update();
    bool occupyLocation(int r, int c);
    bool deoccupyLocation(int r, int c);
    int* getZoneCol(int zone);
    int* getZoneRow(int zone);
    bool isObstacle(int r, int c);

private:
	int grid[15][45];
    int baseGrid[15][45];
    int zoneAShelfCols[6] = { 4, 7, 8, 11, 12, 15 };
    int zoneAShelfRows[3] = { 4, 5, 6 };

    int zoneBShelfCols[6] = { 25, 28, 29, 32, 33, 36 };
    int zoneBShelfRows[3] = { 5, 6, 7 };

    int zoneCShelfCols[6] = { 12, 15, 16, 19, 20, 23 };
    int zoneCShelfRows[3] = { 10, 11, 12 };

    int zoneDShelfCols[6] = { 30, 33, 34, 37, 38, 41 };
    int zoneDShelfRows[3] = { 10, 11, 12 };

    int zoneAAisleCols[6] = { 5, 6, 9, 10, 13, 14 };
    int zoneBAisleCols[6] = { 26, 27, 30, 31, 34, 35 };
    int zoneCAisleCols[6] = { 13, 14, 17, 18, 21, 22 };
    int zoneDAisleCols[6] = { 31, 32, 35, 36, 39, 40 };

    void setShelf(int rows[], int rowSize, int cols[], int colSize);
    void setAisle(int rows[], int rowSize, int cols[], int colSize);
};