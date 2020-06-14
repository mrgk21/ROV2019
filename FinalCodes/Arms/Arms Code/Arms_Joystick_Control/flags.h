#define MODE 1 /*JOYSTICK 0
               	 MANUAL   1 */

#define debug_manual(x) if(MODE){x}
#define debug_joystick(x) if(!MODE){x}

#define DEBUG_SERIAL Serial
#define debug_serial(x) {DEBUG_SERIAL.println(x);DEBUG_SERIAL.flush();}


#define FORMAT "%d_%d%c"

String a = "";
int dof;
int linkIndex;
char motion;
