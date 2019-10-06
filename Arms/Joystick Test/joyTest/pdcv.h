#ifndef PDCV_H
#define PDCV_H

#include "Arduino.h"
class PID;


#define OUTPUT_LIMIT_MIN_POS 90
#define OUTPUT_LIMIT_MAX_POS 255

#define OUTPUT_LIMIT_MIN_NEG 70
#define OUTPUT_LIMIT_MAX_NEG 255


class pdcv
{
    static int delay_ms;
    int pin1;
    int pin2;
    bool reverseDir = false;

    //PID Variables
    int analogPin;
    double Setpoint, Input, Output;
    double Kp = 2, Ki = 0, Kd = 1;
    PID *myPID_pos;
    PID *myPID_neg;
    bool OutputNegative;
  public:
    pdcv(int p1, int p2, bool reverse);
    pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos);
    pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos,double _Kp_neg, double _Kd_neg);
    void pdcv_setup();
    void pdcv_forward(bool backToStatic=true);
    void pdcv_backward(bool backToStatic=true);
    void pdcv_static();
    void pdcv_PID_dual();
    void pdcv_PID();
    void pdcv_setPoint(double setPoint);
    void pdcv_setSpeed(double speed);
    static void setDelay(int);
};
#endif
