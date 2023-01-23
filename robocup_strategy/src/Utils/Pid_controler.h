
#ifndef ROBOCUP_STRATEGY_PID_CONTROLER_H
#define ROBOCUP_STRATEGY_PID_CONTROLER_H
#include <math.h>

class Pid_controler {
public:
    Pid_controler();
    ~Pid_controler();
    double diff_to_output(double diff);
protected:
    double kp;
    double ki;
    double kd;
    double sum;
    double p_error;
    double i_error;
    double d_error;
    double pre_diff;
};


#endif //ROBOCUP_STRATEGY_PID_CONTROLER_H
