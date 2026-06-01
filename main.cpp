#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Layout.h"
#include "OrderManager.h"
#include "Navigation.h"
#include "bstree.h"

using namespace std;

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

void orderMenu(OrderManager& orderManager) {
    int orderChoice;
    int displayOrderChoice;
    std::string customerName;
    std::string itemName;
    while (true) {
        std::cout << "============================================" << std::endl;
        std::cout << "=================ORDER MENU=================" << std::endl;
        std::cout << "============================================" << std::endl;
        std::cout << "1. Add Orders" << std::endl;
        std::cout << "2. Display Orders" << std::endl;
        std::cout << "3. Display Queue Count" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> orderChoice;
        std::cout << " " << std::endl;

        switch (orderChoice) {
            case 1: {
                std::cout << "Customer Name: ";
                std::cin >> customerName;
                // For loop display item
                std::cout << "Item Name: ";
                std::cin >> itemName;
                orderManager.addOrder(customerName, itemName);
                std::cout << "Order has been successfully added!" << std::endl;
                std::cout << " " << std::endl;
                break;
            }
            case 2: {
                std::cout << "============================================" << std::endl;
                std::cout << "=============DISPLAY ORDER MENU=============" << std::endl;
                std::cout << "============================================" << std::endl;
                std::cout << "1. Display Pending Orders" << std::endl;
                std::cout << "2. Display In Progress Orders" << std::endl;
                std::cout << "3. Display Completed Orders" << std::endl;
                std::cout << "4. Display All Orders" << std::endl;
                std::cout << "5. Back" << std::endl;
                std::cout << "Choice: ";
                std::cin >> displayOrderChoice;
                if (displayOrderChoice == 5) { return; }
                orderManager.displayOrders(displayOrderChoice);
                break;
            }
            case 3: {
                std::cout << "Current Order Queue Count: " << orderManager.getQueueCount() << std::endl;
                break;
            }
            case 4: { return; }
        }
    }
}

// Task 4 - Item Search and Management Menu
void itemMenu(BST& itemDB) {
    int choice;
    do {
        std::cout << "=============================================" << std::endl;
        std::cout << "=========ITEM SEARCH & MANAGEMENT============" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "1. Display All Items (Sorted by ID)" << std::endl;
        std::cout << "2. Search Item by ID" << std::endl;
        std::cout << "3. Search Item by Name" << std::endl;
        std::cout << "4. Insert New Item" << std::endl;
        std::cout << "5. Update Item" << std::endl;
        std::cout << "6. Delete Item" << std::endl;
        std::cout << "7. Back" << std::endl;
        std::cout << "Choice: ";
        std::cin >> choice;
        std::cout << " " << std::endl;

        switch (choice) {
        case 1: {
            itemDB.displaySorted();
            break;
        }
        case 2: {
            std::string id;
            std::cout << "Enter Item ID: ";
            std::cin >> id;
            itemDB.searchByID(id);
            break;
        }
        case 3: {
            std::string name;
            std::cin.ignore();
            std::cout << "Enter Item Name: ";
            std::getline(std::cin, name);
            itemDB.searchByName(name);
            break;
        }
        case 4: {
            Item newItem;
            std::cin.ignore();
            std::cout << "Enter Item ID   : "; std::getline(std::cin, newItem.id);
            std::cout << "Enter Item Name : "; std::getline(std::cin, newItem.name);
            std::cout << "Enter Zone      : "; std::getline(std::cin, newItem.zone);
            std::cout << "Enter Aisle     : "; std::getline(std::cin, newItem.aisle);
            std::cout << "Enter Shelf     : "; std::getline(std::cin, newItem.shelf);
            itemDB.insert(newItem);
            break;
        }
        case 5: {
            std::string id;
            std::cout << "Enter Item ID to update: ";
            std::cin >> id;
            itemDB.update(id);
            break;
        }
        case 6: {
            std::string id;
            std::cout << "Enter Item ID to delete: ";
            std::cin >> id;
            itemDB.remove(id);
            break;
        }
        case 7: { break; }
        default: {
            std::cout << "[ERROR] Invalid choice. Please try again." << std::endl;
        }
        }

    } while (choice != 7);
}

int main(){

    Layout layout;
    Navigation* navigation = new Navigation(layout);
    Robot robot;
    robot.currentCol = 0;
    robot.currentRow = 14;
    navigation->moveRobot(&robot, 1, 1, 2);

    int capacity;
    OrderManager orderManager;
    BST itemDB;

    cout << "Enter the number of robots in warehouse: ";
    cin >> capacity;
    RobotManager robotManager(capacity);
    int mainMenuChoice;
    bool exitSystem = false;
    while (true) {
        if (exitSystem) { break; }
        std::cout << "=============================================" << std::endl;
        std::cout << "==================Main MENU==================" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "1. Manage Orders" << std::endl;
        std::cout << "2. Manage Robots" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> mainMenuChoice;
        std::cout << " " << std::endl;

        switch (mainMenuChoice) {
            case 1: {
                orderMenu(orderManager);
                break;
            }
            case 2: {
                robotMenu(robotManager);
                break;
            }
            case 3: {
                itemMenu(itemDB);
            }
            case 4: {
                exitSystem = true;
            }
        }
    }
    
}