

#ifndef ROBOCUP_STRATEGY_TASK_BASE_H
#define ROBOCUP_STRATEGY_TASK_BASE_H
enum Task_state
{
    TASK_START,
    TASK_FINISH
};

class Task_base {
public:
    Task_base();
    ~Task_base();
    Task_state get_task_state();
    virtual void task_run();
protected:
    Task_state task_state;
};


#endif //ROBOCUP_STRATEGY_TASK_BASE_H
