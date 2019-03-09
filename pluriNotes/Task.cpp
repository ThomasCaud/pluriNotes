#include "Task.h"
#include <iostream>

/**
 * @brief Task::display Pour le developpements
 */
void Task::display()const{
    std::cout<<"Note title: "<<getTitle()<<", Task- action: "<<getAction()<<", priority:"<<getPriority()<<", deadline:"<<getDeadline()<<", status:"<<getStatus()<<endl;
}

/**
 * @brief Task::getTypeNote
 * @return
 */
Enumeration::typeNote Task::getTypeNote()const{
    return Enumeration::TASK;
}
