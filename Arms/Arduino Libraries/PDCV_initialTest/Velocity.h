#ifndef VELOCITY_H
#define VELOCITY_H
#include "Arduino.h"
#include "pdcv.h"
class PID;

#define SamplingTime 500
#define tolerance 10

class PDCV_Velocity
{
  private:
    PID* myPID;
    pdcv *_pdcv;
    //PID vars
    double Setpoint,Output,Input;

    int pdcvOut=0;
    //Current and Prev variables
    int currCount,prevCount;
    uint32_t currTime,prevTime;
    
  public:
    PDCV_Velocity() {}
    PDCV_Velocity(pdcv* _myPDCV,double velo,double _Kp, double _Kd);

    void PDCV_velocityControl(int count);
    
};
#endif
