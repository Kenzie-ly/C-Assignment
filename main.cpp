#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Navigation.h"
#include "Layout.h"

using namespace std;

int main(){
    Layout layout;
    Navigation* navigation = new Navigation(layout);
    Robot robot;
    robot.currentCol = 0;
    robot.currentRow = 14;
    navigation->moveRobot(&robot, 1, 1, 2);
    
    cout << "Enter the number of robots in warehouse: ";
    int capacity;
    cin >> capacity;
    RobotManager* robotManager = new RobotManager(capacity);

    robotManager->robotMenu();
    
}
