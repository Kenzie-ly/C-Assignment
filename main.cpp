#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Stack.h"
#include "Layout.h"

int main(){
    using namespace std;

    cout << "Enter the number of robots in warehouse: ";
    int capacity;
    cin >> capacity;
    RobotManager* robotManager = new RobotManager(capacity);

    robotManager->robotMenu();
    
}
