#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Stack.h"

void robotMenu(RobotManager& robotManager){
    using namespace std;

    while (true){
        cout << endl;
        cout << "1. Allocate tasks" << endl;
        cout << "2. Complete tasks" << endl;
        cout << "3. Set maintaincance status" << endl;
        cout << "4. List robot assignment" << endl; //task id, robit id
        cout << "5. Display active tasks" << endl; //task id, robit id, in progress status
        cout << "6. Display robot status" << endl; //robot id, cur task, status, num of completed task

        cout << "7. Exit" << endl;
        cout << "Input: ";
        int choice;
        cin >> choice;
        
        if (choice == 1){
            string task_id;

            cout << "Enter task id: ";
            cin >> task_id;

            if (robotManager.assignTask(task_id)) cout << "Successfully assigining task!" << endl; else cout << "Sorry, no robots are available now!"<<endl;
        }
        else if(choice == 2) robotManager.completeTask();
        else if (choice == 3){
            char id;
            int num;
            char answer;

            cout << "Enter robot id (e.g R1): ";
            cin >> id >> num;
            cout << "Set it into Mantenance (y/n): ";
            cin >> answer;
            
            string r_id = string(1,toupper(id)) + to_string(num);
            bool status = (tolower(answer) == 'y') ? true : false; 
            robotManager.setMaintenanceStatus(r_id, status);
        }
        else if (choice == 4) robotManager.displayAssignmentList();
        else if (choice == 5) robotManager.displayActiveTask();
        else if (choice == 6) robotManager.displayRobotStatus(); 
        else if (choice == 7) break;
    }
}

int main(){
    using namespace std;

    cout << "Enter the number of robots in warehouse: ";
    int capacity;
    cin >> capacity;
    RobotManager robotManager(capacity);

    robotMenu(robotManager);
    
}
