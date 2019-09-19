# PID Library dependency
Download this library for this PDCV library to work properly.

get it here: [PID_v1](https://github.com/br3ttb/Arduino-PID-Library)

# PDCV Library
The library contains :

1. [pdcv.h](https://github.com/mrgk21/ROV2019/blob/VedangJavdekar/Arms/Arduino%20Libraries/PDCV_initialTest/pdcv.h)
2. [pdcv.cpp](https://github.com/mrgk21/ROV2019/blob/VedangJavdekar/Arms/Arduino%20Libraries/PDCV_initialTest/pdcv.cpp)


Make an object of `class pdcv` in your code for using this library.

## Usage
---

**<a id='constructors'></a>Constructors:** 
```c
pdcv(int p1, int p2, bool reverse);
```
-> This has no _PID_ used and the arm acts based on the speed set using `pdcv_setSpeed()` function.

```c
pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos);
```
-> This uses a single _PID_ for both the arm movements(forward/backward).

```c
pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos,double _Kp_neg, double _Kd_neg);
```
-> This uses different _PID_ for both the arm movements(one for pos(forward) and one for neg(backward)).

## Functions:
1. [void pdcv_setup()](#pdcvSetup)
2. [void pdcv_forward(bool backToStatic=true)](#pdcvForward)
3. [void pdcv_backward(bool backToStatic=true)](#pdcvBackward)
4. [void pdcv_static()](#pdcvStatic)
5. [void pdcv_PID_dual()](#pdcvPID_d)
6. [void pdcv_PID()](#pdcvPID)
7. [void pdcv_setPoint(double setPoint)](#pdcvSetPoint)
8. [void pdcv_setSpeed(double speed)](#pdcvSpeed)
9. [static void setDelay(int)](#pdcvDelay)

---

### **<a id='pdcvSetup'></a>void pdcv_setup()**
+ This function is to be called in the `void Setup()` of arduino code. It is used for initialising the pins and PIDs if used in constructor.

### **<a id='pdcvForward'></a>void pdcv_forward(bool backToStatic=true)**
+ This will make the PDCV go in forward direction in accordance with `reverseDir` boolean. The `backToStatic` boolean will make the PDCV static after the delay set using `pdcv::setDelay()` if set to `true`.


### **<a id='pdcvBackward'></a>void pdcv_backward(bool backToStatic=true)**
+ This will make the PDCV go in backward direction in accordance with `reverseDir` boolean. The `backToStatic` boolean will make the PDCV static after the delay set using `pdcv::setDelay()` if set to `true`.

### **<a id='pdcvStatic'></a>void pdcv_static()**
+ This will make the PDCV static. You won't need to explicitly call this function.

### **<a id='pdcvPID_d'></a>void pdcv_PID_dual()**
+ As mentioned in the [constructors](#constructors) this function should be used whenever you are using 2 PIDs for correction. Call this function in `void Loop()` in the arduino code.

### **<a id='pdcvPID'></a>void pdcv_PID()**
+ As mentioned in the [constructors](#constructors) this function should be used whenever you are using a single PID for correction. Call this function in `void Loop()` in the arduino code.

### **<a id='pdcvSetPoint'></a>void pdcv_setPoint(double setPoint)**
+ You can manually set the `Setpoint` by calling this function. Use this in `void Loop()` in your arduino code.

### **<a id='pdcvSpeed'></a>void pdcv_setSpeed(double speed)**
+ Set the speed of the _PDCV_ using this function. It internally sets the variable `Output` in used in the PIDs and for setting the _duty cycle_ of PWM.

### **<a id='pdcvDelay'></a>static void setDelay(int)**
+ This is a static method of the class to set the delay used in the `pdcv_forward()` and `pdcv_backward()` functions if the `backToStatic` boolean is set.
