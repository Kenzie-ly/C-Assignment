#include "RobotManager.h"
#include <iostream>
#include <iomanip>
#include "Navigation.h"

RobotManager::RobotManager(int capacity, Navigation* nav) {
    Robot robot;
    this->nav = nav;

    current_index = -1;
    last_robot_busy_index = -1;
    task_record_head = NULL;
    task_record_tail = NULL;
    total_robots = capacity;
    int row = capacity-1;

    robots = new Robot[total_robots];
    for (int i=0; i<total_robots; i++){
        
        robot.r_id = "R" + std::to_string(i+1);
        robot.r_status = 1;
        robot.numOfTask = 0;
        robot.current_task = "";
        robot.movementLogs = new std::string[200];
        robot.stack = new Stack(200);
        robot.currentCol = 0;
        robot.currentRow = row;
        row--;

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

Robot RobotManager::assignTask(std::string task, Location location) {
    //enqueue at rear
    int new_index = (last_robot_busy_index+1) % total_robots;

    //maintenance
    if (robots[new_index].r_status == -1){
        bool isFound = false;

        for (int i = 0; i < total_robots; i++) {
            //start from last_robot
            if (robots[new_index].r_status == 1) {
                isFound = true;
                break;
            }
            new_index = (new_index + 1) % total_robots;
        }
        if(!isFound) return Robot();
    }

    if(current_index == -1 || is_empty) {
        //empty case
        current_index = new_index;
        is_empty = false;
    }else if (new_index == current_index && !is_empty){
        //means full, not empty
        return Robot();
    }

    last_robot_busy_index = new_index;


    AssignmentRecord* newRecord = new AssignmentRecord();
    newRecord->robot = robots[last_robot_busy_index];
    newRecord->t_id = task;
    newRecord->t_status = 0;
    newRecord->targetLocation = location;
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
    robots[last_robot_busy_index].current_target_location = location;

    return robots[last_robot_busy_index];
}

void RobotManager::completeTask() {
    if(is_empty || current_index == -1){
        std::cout << "There are no tasks to be completed!" << std::endl;
        return;
    };

    //dequeue at front
    robots[current_index].current_task = "";
    robots[current_index].r_status = 1;

    //update assignment record
    AssignmentRecord* cur = task_record_head;
    while (cur != NULL){
        if(cur->robot.r_id == robots[current_index].r_id && cur->t_status == 0){
            cur->t_status = 1;
            std::cout<< "Sucesfully completed task for " << cur->t_id << std::endl;
            break;
        }
        cur = cur->nextRecord;
    }

    if (current_index != last_robot_busy_index){ 
        current_index = ((current_index+1) % total_robots);
    }else{
        is_empty = true;
    }
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
            
    cout << "Robot Status..." << endl;
    cout << left << setw(15) << "Robot ID" 
            << setw(18) << "Current Task" 
            << setw(15) << "Status" 
            << setw(25) << "Number of Assigned Tasks" << endl;
    
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
                << setw(15) << cur->robot.r_id 
                << setw(15) << checkStatus(cur->t_status, 't') << endl;
        cur = cur->nextRecord;
    }
}

void RobotManager::displaySelectedRobotAssignmentList(Robot* robot)
{
    if (task_record_head == NULL) {
        std::cout << "There are no tasks assigned yet." << std::endl;
        return;
    }

    AssignmentRecord* cur = task_record_head;

    while (cur != nullptr) {
        if (cur->robot.r_id == robot->r_id)
        {
            std::cout << std::left << std::setw(15) << cur->t_id;
            cur = cur->nextRecord;
        }
    }
}

std::string RobotManager::checkStatus(int status, char code) {
    using namespace std;
    string s = "";

    if(toupper(code) == 'R'){
        if (status == -1) s = "Maintenance";
        else if (status == 0) s = "Busy";
        else if (status == 1) s = "Available";
        return s;
    }else{
        if (status == 0) s = "In Progress";
        else if (status == 1) s = "Completed";
        return s;
    }
}

void RobotManager::setMaintenanceStatus(std::string r_id, bool status){
    for(int i=0; i<total_robots; i++){
        if(robots[i].r_id == r_id){
            if(robots[i].r_status == 0 && status){
                std::cout<< "Cannot set status on running robots!"<<std::endl;
                return;
            }

            if(status){
                robots[i].r_status = -1;
                std::cout << "Successfully changed status into maintenance!" << std::endl;
            }else{
                robots[i].r_status = 1;
                std::cout << "Successfully set into available!" << std::endl;
            }
            
            return;
        }
    }

    std::cout << "Robot ID cannot be found!" << std::endl;
}



AssignmentRecord* RobotManager::getRecord(std::string r_id, std::string task){
    AssignmentRecord* cur = task_record_head;
    
    while (cur != nullptr){
        if(cur->robot.r_id == r_id && cur->t_id == task){

            return cur;
        }
    }
}
