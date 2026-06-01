#pragma once
#include <string>

struct Order {
    int OrderID;
    std::string CustomerName, ItemName, Status;
    Order* NextOrder;

    Order(int orderID, std::string customerName, std::string itemName, std::string status) {
        OrderID = orderID;
        CustomerName = customerName;
        ItemName = itemName;
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
    void addOrder(std::string customerName, std::string itemName);
    Order* getOrder();
    bool isEmpty();
    bool historyIsEmpty();
    int getQueueCount();
    void displayOrders(int choice);
};