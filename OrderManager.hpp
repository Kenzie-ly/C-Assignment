#pragma once
#include <string>
#include "bstree.hpp"

struct Order {
    int OrderID;
    std::string CustomerName, Status;
    Item* ItemNode;
    Order* NextOrder;

    Order(int orderID, std::string customerName, Item* item, std::string status) {
        OrderID = orderID;
        CustomerName = customerName;
        ItemNode = item;
        Status = status;
        NextOrder = nullptr;
    }
};

class OrderManager {
    Order* queueFront;
    Order* queueEnd;
    Order* orderHistoryHead;
    int orderID;
    int queueCount;

    public:
    OrderManager();
    void addOrder(std::string customerName, Item* item);
    Order* getOrder();
    Order* peekOrder();
    bool isEmpty();
    bool historyIsEmpty();
    int getQueueCount();
    void displayOrders(int choice);
    void markCompleted(int orderID);
};