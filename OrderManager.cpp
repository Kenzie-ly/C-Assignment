#include <string>
#include <iostream>
#include "OrderManager.h"
#include "bstree.h"

OrderManager::OrderManager() {
    queueFront = nullptr;
    queueEnd = nullptr;
    orderHistoryHead = nullptr;
    orderID = 1;
    queueCount = 0;
}

void OrderManager::addOrder(std::string customerName, Item* item) {
    std::string status = "Pending";
    Order* newOrder = new Order(orderID, customerName, item, status);
    orderID++;
    if (isEmpty()) {
        queueFront = newOrder;
        queueEnd = newOrder;
    } else {
        queueEnd -> NextOrder = newOrder;
        queueEnd = newOrder;
    }
    queueCount++;
    if (historyIsEmpty()) {
        orderHistoryHead = newOrder;
    }
}

Order* OrderManager::getOrder() {
    if (isEmpty()) {
        return nullptr;
    }
    Order* order = queueFront;
    queueFront = queueFront -> NextOrder;
    if (isEmpty()) {
        queueEnd = nullptr;
    }
    order -> Status = "Processing";
    queueCount--;
    return order;
}

Order* OrderManager::peekOrder() {
    return queueFront;
}

bool OrderManager::isEmpty() {
    if (queueFront == nullptr) {
        return true;
    }
    return false;
}

bool OrderManager::historyIsEmpty() {
    if (orderHistoryHead == nullptr) {
        return true;
    }
    return false;
}

int OrderManager::getQueueCount() {
    return queueCount;
}

void OrderManager::displayOrders(int choice) {
    if (historyIsEmpty()) {
        std::cout << "Order history is empty. Please add orders to see results" << std::endl;
        return;
    }
    std::string statusChoice;
    Order* current = orderHistoryHead; // Since all orders are put inside history upon creation
    std::cout << "===========================================" << std::endl;
    std::cout << "===============ORDER DISPLAY===============" << std::endl;
    std::cout << "===========================================" << std::endl;
    if (choice == 1) {
        statusChoice = "Pending";
    } else if (choice == 2) {
        statusChoice = "Processing";
    } else if (choice == 3) {
        statusChoice = "Completed";
    } else {
        while (current != nullptr) {
            std::cout << "OrderID: " << current->OrderID << std::endl;
            std::cout << "Customer Name: " << current->CustomerName << std::endl;
            std::cout << "Item Name: " << current->ItemNode->name << std::endl;
            std::cout << "Status: " << current->Status << std::endl;
            current = current->NextOrder;
        }
        std::cout << " " << std::endl;
        return; // Returns so that the next print (For order status filters) will work
    }
    while (current != nullptr) {
        if (current->Status == statusChoice) {
            std::cout << "OrderID: " << current->OrderID << std::endl;
            std::cout << "Customer Name: " << current->CustomerName << std::endl;
            std::cout << "Item Name: " << current->ItemNode->name << std::endl;
            std::cout << "Status: " << current->Status << std::endl; // Useless, the status choice has been picked, so this is redundant. Only for testing purposes.
        }
        current = current->NextOrder;
    }
    std::cout << " " << std::endl;
}

// Will try find a better solution than this
void OrderManager::markCompleted(int orderID) {
    // If an order can be marked, it already exists (Guarded in getOrder)
    Order* current = orderHistoryHead;
    while (current != nullptr) {
        if (current->OrderID == orderID) {
            current->Status = "Completed";
            return;
        }
        current = current->NextOrder;
    }
}