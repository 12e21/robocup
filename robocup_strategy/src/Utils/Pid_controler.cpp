
#include "Pid_controler.h"
Pid_controler::Pid_controler() {
    this->sum=0;
    this->pre_diff=0;
    this->p_error=0;
    this->i_error=0;
    this->d_error=0;
    this->kp=0.8;
    this->ki=0;
    this->kd=0.0003;
}
Pid_controler::~Pid_controler()=default;

double Pid_controler::diff_to_output(double diff) {
    p_error=diff;
    i_error=i_error+diff;
    if(abs(i_error)>0.7){
        i_error=0;
    }
    d_error=diff-pre_diff;
    pre_diff=diff;
    sum=(kp*p_error)+(ki*i_error)+(kd*d_error);
    return sum;
}