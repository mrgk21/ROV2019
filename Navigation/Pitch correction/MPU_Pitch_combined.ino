#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#define OUTPUT_READABLE_YAWPITCHROLL
MPU6050 mpu;

#define INTERRUPT_PIN 2  
#define LED_PIN 13 
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float euler[3];   
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}


//towards router pitch positive and lead screw LOW
int PUL=6; //define Pulse pin
int DIR=7; //define Direction pin
int ENA=5; //define Enable Pin

void setup() {
  // put your setup code here, to run once:
   Serial.begin(115200);
   while (!Serial);
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();
    pinMode(INTERRUPT_PIN, INPUT);

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // wait for ready
    Serial.println(F("\nSend any character to begin DMP programming and demo also all the values are in degrees: "));
    while (Serial.available() && Serial.read()); // empty buffer
    while (!Serial.available());                 // wait for data
    while (Serial.available() && Serial.read()); // empty buffer again
    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
    pinMode(LED_PIN, OUTPUT);

    pinMode (PUL, OUTPUT);
    pinMode (DIR, OUTPUT);
    pinMode (ENA, OUTPUT);
}

void loop() {
   if (!dmpReady) return;
   while (!mpuInterrupt && fifoCount < packetSize) {}
   mpuInterrupt = false;
   mpuIntStatus = mpu.getIntStatus();
   fifoCount = mpu.getFIFOCount();
   if (mpuIntStatus & 0x02)
   {    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;

    #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu.dmpGetQuaternion(&q, fifoBuffer);
            mpu.dmpGetGravity(&gravity, &q);
            mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
            //Serial.print("ypr\t");
            //Serial.print(ypr[0] * 180/M_PI);
            Serial.print("PITCH \n");
            Serial.print(ypr[1] * 180/M_PI);
            if (Serial.available()>0)
            {
            String  y = Serial.read();
            int currentpitch = y.toInt();
            }
            //Serial.print("\t");
            //Serial.println(ypr[2] * 180/M_PI);
        #endif
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
     
      int reqpitch = 0;
      error = reqpitch - currentpitch;
      int eps=5;

      while (1)
  {
   if(abs(error)<=eps) //If within limit, do nothing
  {
      digitalWrite(ENA,LOW);
  }

  else  //error out of eps range
  {
     if (error < 0) //cp positive
     {
      while (!(abs(error)<=eps))
      {
        for (int i=0; i<6400; i++)   //Backward 5000 steps
        {
        digitalWrite(DIR,LOW);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(18);
        digitalWrite(PUL,LOW);
        delayMicroseconds(18);
        }
     }
   }
     if (error > 0) //cp negative
     {
      while (!(abs(error)<=eps))
      {
        for (int i=0; i<6400; i++)   //Backward 5000 steps
        {
        digitalWrite(DIR,HIGH);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(18);
        digitalWrite(PUL,LOW);
        delayMicroseconds(18);
        }
     }
     }
  }
  }
   }
   }
