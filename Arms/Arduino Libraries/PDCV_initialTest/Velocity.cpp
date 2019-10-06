#include "Velocity.h"
#include <PID_v1.h>

PDCV_Velocity::PDCV_Velocity(pdcv *myPDCV,double velo, double _Kp, double _Kd):_pdcv(myPDCV),Setpoint(velo)
{
  myPID = new PID(&Input, &Output, &Setpoint, _Kp, 0, _Kd, DIRECT);
  myPID->SetOutputLimits(-255, 255);
  currTime = prevTime = 0;
}

void PDCV_Velocity::PDCV_velocityControl(int counts)
{
  currCount = counts;
  currTime = millis();
  if(currTime - prevTime >= SamplingTime)
  {
    int error = currCount - prevCount;
    if(abs(Setpoint - error) > tolerance)
    {
      myPID->Compute();
      pdcvOut+=Output;
      if(pdcvOut<0) pdcvOut=0;
      else if(pdcvOut >255) pdcvOut=255;
      
      _pdcv->pdcv_setSpeed(pdcvOut);
    }
  }
}


