#pragma once
#include <string>

struct AssignmentRecord {
    std::string r_id;
    std::string t_id;
    int t_status;

    AssignmentRecord* nextRecord;
};

struct Robot {
    std::string r_id;
    int r_status;
    std::string current_task;
    int numOfTask = 0;
    int currentRow;
    int currentCol;
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
    RobotManager(int capacity);

    ~RobotManager();

    bool assignTask(std::string task);

    void completeTask();

    void displayActiveTask();

    void displayRobotStatus();

    void displayAssignmentList();

    void robotMenu();

private:
    std::string checkStatus(int status, char code);
};