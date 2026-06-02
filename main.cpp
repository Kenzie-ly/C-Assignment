#include <iostream>
#include <string>
#include "RobotManager.h"
#include "Layout.h"
#include "OrderManager.h"
#include "Navigation.h"
#include "bstree.h"
#include <sstream>

void processAllOrders(RobotManager& robotManager, OrderManager& orderManager, BST& itemDB){
    using namespace std;

    if (orderManager.isEmpty()){
        cout << "There are no orders currently" << endl;
    }

    while (!orderManager.isEmpty()){
        Order* order = orderManager.peekOrder();

        cout << "\nReading Order..." << endl;
        cout << "OrderID: " << order->OrderID << endl;
        cout << "Item Name: " << order->ItemNode->name << endl;

        cout << "\nAssigning robot..." << endl;
        string order_id = to_string(order->OrderID);

        cout << "\nLocating the item..." << endl;
        std::string location = itemDB.getLocation(order->ItemNode->id);
        std::stringstream split(location);
        std::string zone, aisle, shelf;
        int z = 0, a = 0, s = 0;
        std::getline(split, zone, ',');
        std::getline(split, aisle, ',');
        std::getline(split, shelf, ',');

        if (zone == "Zone-A") z = 1;
        else if (zone == "Zone-B") z = 2;
        else if (zone == "Zone-C") z = 3;
        else if (zone == "Zone-D") z = 4;

        if (aisle == "Aisle-1") a = 1;
        else if (aisle == "Aisle-2") a = 2;
        else if (aisle == "Aisle-3") a = 3;

        if (shelf == "Shelf-1") s = 1;
        else if (shelf == "Shelf-2") s = 2;
        Location targetLocation;
        targetLocation.zone = z;
        targetLocation.aisle = a;
        targetLocation.shelf = s;

        Robot robot = robotManager.assignTask(order_id, targetLocation);
        std::string robot_id = robot.r_id;
        if (robot.r_id == "") {
            cout << "No robot available. Stopping order processing.\n";
            return;
        }

        cout << endl;
        cout << "Assigned Robot ID: " << robot_id << endl;
        robotManager.displayRobotStatus();
        orderManager.getOrder();

        cout << "\nPicking up the item..." << endl;
        cout << "Item Name: " << order->ItemNode->name << endl;

        cout << "\nDelivering the item to packing station..." << endl;

        orderManager.markCompleted(order->OrderID);
        robotManager.completeTask();

        cout << "\n\n\n";
    }
}

void robotMenu(RobotManager& robotManager, OrderManager& orderManager, BST& itemDB){
    using namespace std;

    while (true){
        cout << endl;
        cout << "1. Process all orders" << endl;
        cout << "2. Set maintenance status" << endl;
        cout << "3. Display robot assignment" << endl; //task id, robit id
        cout << "4. Display robot status" << endl; //robot id, cur task, status, num of completed task
        cout << "5. Display robot movement" << endl;

        cout << "6. Exit" << endl;
        cout << "Input: ";
        int choice;
        cin >> choice;
        
        if (choice == 1) processAllOrders(robotManager, orderManager, itemDB);
        else if (choice == 2){
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
        else if (choice == 3) robotManager.displayAssignmentList();
        else if (choice == 4) robotManager.displayRobotStatus();
        else if (choice == 5){
            robotManager.displayRobotStatus();//all r_id

            std::string r_id;
            std::string task;

            cout << "Select robot to display movement:";
            cin >> r_id;
            
            robotManager.displaySelectedRobotAssignmentList(r_id);
            cout << "Select task to display movement:";
            cin >> task;
            
            
            AssignmentRecord* record = robotManager.getRecord(r_id, task);
            robotManager.nav->moveRobot(&record->robot, record->targetLocation.zone, record->targetLocation.aisle, record->targetLocation.shelf);
        } 
        else if (choice == 6) break;
    }
}

void orderMenu(OrderManager& orderManager, BST& itemDB) {
    int orderChoice;
    int displayOrderChoice;
    int itemChoice;
    int totalItem;
    std::string customerName;
    BSTNode* selectedItem = nullptr;
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
                std::cout << "=============================================" << std::endl;
                std::cout << "===============Available Items===============" << std::endl;
                std::cout << "=============================================" << std::endl;
                totalItem = itemDB.getItemCount();
                for (int i = 0; i < totalItem; i++) {
                    BSTNode* node = itemDB.getItemByIndex(i);
                    std::cout << i + 1 << ". " << node->data.name << std::endl;
                }
                std::cout << "Item Choice: ";
                // Match choice with item name
                std::cin >> itemChoice;
                selectedItem = itemDB.getItemByIndex(itemChoice - 1);
                if (selectedItem == nullptr) { break; }
                orderManager.addOrder(customerName, &selectedItem->data);
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
// ============================================================
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

    int capacity;
    OrderManager orderManager;
    BST itemDB;

    std::cout << "Enter the number of robots in warehouse (Max 10): ";
    std::cin >> capacity;

    while(capacity > 10){
        std::cout << "Enter the number of robots in warehouse (Max 10): ";
        std::cin >> capacity;

        if(capacity>10) std::cout << "Cannot be more than 10";
    }
    RobotManager robotManager(capacity, navigation);

    int mainMenuChoice;
    bool exitSystem = false;
    while (true) {
        if (exitSystem) { break; }
        std::cout << "=============================================" << std::endl;
        std::cout << "==================Main MENU==================" << std::endl;
        std::cout << "=============================================" << std::endl;
        std::cout << "1. Manage Orders" << std::endl;
        std::cout << "2. Manage Robots" << std::endl;
        std::cout << "3. Manage Items" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Choice: ";
        std::cin >> mainMenuChoice;
        std::cout << " " << std::endl;

        switch (mainMenuChoice) {
            case 1: {
                orderMenu(orderManager, itemDB);
                break;
            }
            case 2: {
                robotMenu(robotManager, orderManager, itemDB);
                break;
            }
            case 3: {
                itemMenu(itemDB);
                break;
            }
            case 4: {
                exitSystem = true;
            }
        }
    }
    delete navigation;
    return 0;
    
}