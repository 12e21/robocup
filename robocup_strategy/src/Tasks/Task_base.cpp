
#include "Task_base.h"
Task_base::Task_base() {
    this->task_state=TASK_START;
}
Task_base::~Task_base()=default;

Task_state Task_base::get_task_state() {
    return this->task_state;
}

void Task_base::task_run() {}