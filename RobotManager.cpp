#include "RobotManager.h"
#include <iostream>
#include <iomanip>

using namespace std;

RobotManager::RobotManager(int capacity) {
    Robot robot;

    current_index = -1;
    last_robot_busy_index = -1;
    task_record_head = NULL;
    task_record_tail = NULL;
    total_robots = capacity;

    robots = new Robot[total_robots];
    for (int i=0; i<total_robots; i++){
        
        robot.r_id = "R" + std::to_string(i+1);
        robot.r_status = 1;
        robot.numOfTask = 0;
        robot.current_task = "";

        robots[i] = robot;
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
    int new_index = (last_robot_busy_index+1) % total_robots;

    if(current_index == -1 || is_empty) {
        //empty case
        current_index = (current_index+1) % total_robots;
        is_empty = false;
    }else if (new_index == current_index && !is_empty){
        //means full, not empty
        return false;
    }else if (robots[new_index].r_status == -1){
        int index = new_index;
        bool isFound = false;

        while (index != current_index){
            if(robots[index].r_status == 1){
                isFound = true;
                break;
            }
            index = (index+1) % total_robots;
        }
        
        if(!isFound) return false;
        last_robot_busy_index = index;
    }

    last_robot_busy_index = new_index;

    AssignmentRecord* newRecord = new AssignmentRecord();
    newRecord->r_id = robots[last_robot_busy_index].r_id;
    newRecord->t_id = task;
    newRecord->t_status = 0;
    newRecord->nextRecord = nullptr;

    if (task_record_head == nullptr){
        task_record_head = newRecord;
        task_record_tail = newRecord;
    } else {
        task_record_tail->nextRecord = newRecord;
        task_record_tail = newRecord;
    }

    robots[last_robot_busy_index].r_status = 0;
    robots[last_robot_busy_index].current_task = task;
    robots[last_robot_busy_index].numOfTask += 1;

    return true;
}

void RobotManager::completeTask() {
    if(is_empty){
        std::cout << "There are no tasks to be completed!" << std::endl;
        return;
    };

    //dequeue at front
    robots[current_index].current_task = "";
    robots[current_index].r_status = 1;

    //update assignment record
    AssignmentRecord* cur = task_record_head;
    while (cur != NULL){
        if(cur->r_id == robots[current_index].r_id && cur->t_status == 0){
            cur->t_status = 1;
            break;
        }
        cur = cur->nextRecord;
    }

    if (current_index != last_robot_busy_index){ 
        current_index = ((current_index+1) % total_robots);
    }else{
        is_empty = true;
    }

    std::cout<< "Sucesfully completed task, " << cur->t_id << std::endl;
}

void RobotManager::displayActiveTask() {
    using namespace std;

    if(last_robot_busy_index == -1){
        cout << "No active tasks currently!" << endl;

        return;
    }

    bool isFound = false;

    for (int i=0; i < total_robots; i++ ){
        
        if(robots[i].r_status == 0){
            if(!isFound){
                cout << "Active Task" << endl;
                cout << left << setw(15) << "Task ID" 
                        << setw(15) << "Robot ID" 
                        << setw(15) << "Status" << endl;
                isFound = true;
            }
            
            cout << left << setw(15) << this->robots[i].current_task 
                    << setw(15) << this->robots[i].r_id 
                    << setw(15) << "In Progress" << endl;
        }
    }

    if(!isFound) cout << "No active task currently!" << endl;
}

void RobotManager::displayRobotStatus() {
    using namespace std;
            
    cout << "Robot Status" << endl;
    cout << left << setw(15) << "Robot ID" 
            << setw(18) << "Current Task" 
            << setw(15) << "Status" 
            << setw(30) << "Number of Tasks Completed" << endl;
    
    for (int i=0; i <total_robots; i++ ){

        cout << left << setw(15) << robots[i].r_id 
                << setw(18) << ((robots[i].current_task == "") ? "None" : robots[i].current_task) 
                << setw(15) << checkStatus(robots[i].r_status, 'r') 
                << setw(30) << robots[i].numOfTask << endl;
    }
}

void RobotManager::displayAssignmentList() {
    using namespace std;

    if(task_record_head == NULL){ 
        cout << "There are no tasks assigned yet." <<endl;
        return;
    }

    AssignmentRecord* cur = task_record_head;
    
    cout << left << setw(15) << "Task ID" 
            << setw(15) << "Robot ID" 
            << setw(15) << "Task Status" << endl;
    while (cur != nullptr){
        cout << left << setw(15) << cur->t_id 
                << setw(15) << cur->r_id 
                << setw(15) << checkStatus(cur->t_status, 't') << endl;
        cur = cur->nextRecord;
    }
}

std::string RobotManager::checkStatus(int status, char code) {
    using namespace std;
    string s = "";

    if(toupper(code) == 'R'){
        if (status == -1) s = "Maintainance";
        else if (status == 0) s = "Busy";
        else if (status == 1) s = "Available";
        return s;
    }else{
        if (status == 0) s = "In Progress";
        else if (status == 1) s = "Completed";
        return s;
    }
}