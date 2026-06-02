#pragma once
#include <string>
#include "Stack.h"

class Navigation;

struct Location {
    int zone;
    int aisle;
    int shelf;
};

struct Robot {
    std::string r_id;
    int r_status;
    std::string current_task;
    int numOfTask = 0;
    int currentRow;
    int currentCol;
    Location current_target_location;

    //robot movement
    Stack* stack;
    int top = -1;
    std::string* movementLogs;
};

struct AssignmentRecord {
    //robot record, containing(robot id, task id, task status)
    Robot robot;    
    std::string t_id;
    Location targetLocation;
    int t_status;

    AssignmentRecord* nextRecord;
};

class RobotManager {
    Robot* robots;//heap array

    AssignmentRecord* task_record_head;
    AssignmentRecord* task_record_tail;

    int current_index; //front
    int last_robot_busy_index; //rear

    int total_robots;
    bool is_empty = true;

    public:
        RobotManager(int capacity, Navigation* nav);

        ~RobotManager();

        Robot assignTask(std::string task, Location location);

        void completeTask();

        void displayActiveTask();

        void displayRobotStatus();

        void displayAssignmentList();

        void displaySelectedRobotAssignmentList(Robot* robot);

        void setMaintenanceStatus(std::string r_id, bool status);

        AssignmentRecord* getRecord(std::string r_id, std::string task);

        Navigation* nav;

    private:
        std::string checkStatus(int status, char code);
};