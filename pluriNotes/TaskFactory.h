#ifndef TASKFACTORY_H
#define TASKFACTORY_H

#include "Task.h"

/**
 * @brief The TaskFactory class implémente le design pattern Factory. Elle permet de créer concretement des objets Tache
 */
class TaskFactory{
public:
    TaskFactory(){}
    Task* createTask(const string& title){return new Task(title,"NoAction",Enumeration::UNDEFINEDPRIORITY);}
};

#endif // TASKFACTORY_H
