#ifndef PDCV_H
#define PDCV_H

#include "Arduino.h"

class pdcv
{
  static int delay_ms;
	int pin1;
	int pin2;
	bool reverseDir = false;
public:
	pdcv(int,int,bool);
	void pdcv_setup();
	void pdcv_forward();
	void pdcv_backward();
	void pdcv_static();
  static void setDelay(int);
};

#endif
