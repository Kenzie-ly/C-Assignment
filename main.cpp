#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Stack.h"
#include "Layout.h"

using namespace std;

int main(){
    Layout* layout = new Layout();
    layout->display();

    cout << "Enter the number of robots in warehouse: ";
    int capacity;
    cin >> capacity;
    RobotManager* robotManager = new RobotManager(capacity);

    robotManager->robotMenu();
    
}
