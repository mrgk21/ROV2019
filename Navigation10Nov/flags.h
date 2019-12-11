/************** DEBUG FLAGS ****************/
#define mySerial Serial
#define THR_ON 1
#define PUMP_ON 1 //THIS IS  temp
#define STEPPER_ON 1

#define debug_thr(x) if(THR_ON){x}
#define debug_pump(x) if(PUMP_ON){x}
#define debug_step(x) if(STEPPER_ON){x}

/*************** CONSTANTS **************/
enum THRUST {UP_1, UP_2, UP_3, FWD_1, FWD_2, HORZ} thr;

#define T_POS 1700
#define T_NEG 1300
#define T_ZERO 1500

#define PITCH_TOLERANCE 10
#define PITCH_OFFSET 0

#define STEPPER_EN A1
#define STEPPER_PUL 9
#define STEPPER_DIR A0
#define STEPPER_MAXSPEED 1600
#define STEPPER_SPEED 1500
