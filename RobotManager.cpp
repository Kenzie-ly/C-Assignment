#include "RobotManager.h"
#include <iostream>

using namespace std;

RobotManager::RobotManager(int capacity) {
    Robot robot;

    current_index = -1;
    last_robot_busy_index = -1;
    task_record_head = NULL;
    task_record_tail = NULL;
    total_robots = capacity;

    this->robots = new Robot[total_robots];
    for (int i = 0; i < total_robots; i++) {

        robot.r_id = "R" + std::to_string(i + 1);
        robot.r_status = 1;
        robot.numOfTask = 0;
        robot.current_task = "";

        this->robots[i] = robot;
    }
}

RobotManager::~RobotManager() {
    delete[] this->robots;

    AssignmentRecord* cur = task_record_head;
    while (cur != NULL) {
        cur = cur->nextRecord;

        delete task_record_head;
        task_record_head = cur;
    }
}

bool RobotManager::assignTask(std::string task) {
    //enqueue at rear
    bool same_position = ((last_robot_busy_index + 1) % total_robots) == (current_index + 1 % total_robots);

    if (same_position && task_record_head != NULL) {
        return false;
    }
    last_robot_busy_index += 1;
    last_robot_busy_index = last_robot_busy_index % total_robots;

    if (robots[last_robot_busy_index].r_status == -1) {
        int index = last_robot_busy_index;
        bool isFound = false;

        while (index != current_index) {
            if (robots[index].r_status == 1) {
                break;
            }
            index = (index + 1) % total_robots;
        }

        if (!isFound) return false;
        last_robot_busy_index = index;
    }

    AssignmentRecord* newRecord = new AssignmentRecord();
    newRecord->r_id = robots[last_robot_busy_index].r_id;
    newRecord->t_id = task;
    newRecord->t_status = 0;
    newRecord->nextRecord = nullptr;

    if (task_record_head == nullptr) {
        task_record_head = newRecord;
        task_record_tail = newRecord;
    }
    else {
        task_record_tail->nextRecord = newRecord;
        task_record_tail = newRecord;
    }

    robots[last_robot_busy_index].r_status = 0;
    robots[last_robot_busy_index].current_task = task;
    robots[last_robot_busy_index].numOfTask += 1;

    return true;
}

void RobotManager::completeTask() {
    bool same_position = ((last_robot_busy_index + 1) % total_robots) == (current_index + 1 % total_robots);

    //dequeue at front
    if (same_position || task_record_head == NULL) {
        std::cout << "There are no tasks to be completed!";
        return;
    }

    current_index += 1;
    current_index = current_index % total_robots;

    robots[current_index].current_task = "";
    robots[current_index].r_status = 1;

    //update assignment record
    AssignmentRecord* cur = task_record_head;
    while (cur != NULL) {
        if (cur->r_id == robots[current_index].r_id) {
            cur->t_status = 1;
            break;;
        }
        cur = cur->nextRecord;
    }

    std::cout << "Sucesfully completed task, " << cur->t_id << std::endl;
}

void RobotManager::displayActiveTask() {
    if (last_robot_busy_index == -1) {
        cout << "No active tasks currently!" << endl;

        return;
    }

    bool isFound = false;

    for (int i = 0; i < total_robots; i++) {

        if (robots[i].r_status == 0) {
            if (!isFound) {
                cout << "Active Task" << endl;
                cout << "Task ID\t\tRobot ID\t\tStatus\n";
                isFound = true;
            }

            cout << this->robots[i].current_task << "\t\t" << this->robots[i].r_id << "\t\t" << "In Progress" << endl;
        }
    }

    if (!isFound) cout << "No active task currently!" << endl;
}

void RobotManager::displayRobotStatus() {
    cout << "Robot Status" << endl;
    cout << "Robot ID\t\tCurrent Task\t\tStatus\t\tNumber of Tasks Completed\n";

    for (int i = 0; i < total_robots; i++) {

        cout << robots[i].r_id
            << "\t\t"
            << ((robots[i].current_task == "") ? "None" : robots[i].current_task)
            << "\t\t"
            << checkStatus(robots[i].r_status, 'r')
            << "\t\t"
            << robots[i].numOfTask << endl;
    }
}

void RobotManager::displayAssignmentList() {
    if (task_record_head == NULL) {
        cout << "There are no tasks assigned yet." << endl;
        return;
    }

    AssignmentRecord* cur = task_record_head;

    cout << "Task ID\t\tRobot ID\t\tTask Status\n";
    while (cur != nullptr) {
        cout << cur->t_id << "\t\t" << cur->r_id << checkStatus(cur->t_status, 't') << endl;
        cur = cur->nextRecord;
    }
}

std::string RobotManager::checkStatus(int status, char code) {
    string s = "";

    if (toupper(code) == 'R') {
        if (status == -1) s = "Maintainance";
        else if (status == 0) s = "Busy";
        else if (status == 1) s = "Available";
        return s;
    }
    else {
        if (status == 0) s = "In Progress";
        else if (status == 1) s = "Completed";
        return s;
    }
}