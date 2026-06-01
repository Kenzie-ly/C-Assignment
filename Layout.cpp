#include "Layout.h"
#include <iostream>

Layout::Layout()
{
    for (int r = 0; r < 15; r++)
        for (int c = 0; c < 45; c++)
            grid[r][c] = WALKABLE;

    for (int r = 0; r < 15; r++)
        for (int c = 0; c < 45; c++)
            if (r >= 3 && r <= 7 && c >= 3 && c <= 16)
                grid[r][c] = ZONE_A;

            else if (r >= 4 && r <= 8 && c >= 24 && c <= 37)
                grid[r][c] = ZONE_B;

            else if (r >= 9 && r <= 13 && c >= 11 && c <= 24)
                grid[r][c] = ZONE_C;

            else if (r >= 9 && r <= 13 && c >= 29 && c <= 42)
                grid[r][c] = ZONE_D;

            else if (((r >= 12 && r <= 14) && (c >= 4 && c <= 7)) || 
                ((r >= 8 && r <= 9) && (c >= 8 && c <= 10)) || 
                ((r >= 0 && r <= 1) && (c >= 0 && c <= 2)) || 
                (r == 4 && (c >= 42 && c <= 44)))
                grid[r][c] = OBSTACLE;

    setShelf(zoneAShelfRows, 3, zoneAShelfCols, 6);
    setShelf(zoneBShelfRows, 3, zoneBShelfCols, 6);
    setShelf(zoneCShelfRows, 3, zoneCShelfCols, 6);
    setShelf(zoneDShelfRows, 3, zoneDShelfCols, 6);

    setAisle(zoneAShelfRows, 3, zoneAAisleCols, 6);
    setAisle(zoneBShelfRows, 3, zoneBAisleCols, 6);
    setAisle(zoneCShelfRows, 3, zoneCAisleCols, 6);
    setAisle(zoneDShelfRows, 3, zoneDAisleCols, 6);
}

void Layout::setShelf(int rows[], int rowSize, int cols[], int colSize)
{
    for (int r = 0; r < rowSize; r++)
        for (int c = 0; c < colSize; c++)
            grid[rows[r]][cols[c]] = SHELF;
}

void Layout::setAisle(int rows[], int rowSize, int cols[], int colSize)
{
    for (int r = 0; r < rowSize; r++)
        for (int c = 0; c < colSize; c++)
            grid[rows[r]][cols[c]] = AISLE;
}

void Layout::display()
{
    for (int r = 0; r < 15; r++)
    {
        for (int c = 0; c < 45; c++)
        {
            switch (grid[r][c]) {
            case WALKABLE: std::cout << "\033[100m  \033[0m"; break;  // gray
            case OBSTACLE: std::cout << "\033[46m  \033[0m"; break; // dark red
            case ZONE_A:   std::cout << "\033[41m  \033[0m"; break;   // red
            case ZONE_B:   std::cout << "\033[42m  \033[0m"; break;   // green
            case ZONE_C:   std::cout << "\033[44m  \033[0m"; break;   // blue
            case ZONE_D:   std::cout << "\033[43m  \033[0m"; break;   // yellow
            case SHELF:    std::cout << "\033[40m  \033[0m"; break;   // black
            case AISLE:    std::cout << "\033[47m  \033[0m"; break;   // white
            }
        }
        std::cout << '\n';
    }
}