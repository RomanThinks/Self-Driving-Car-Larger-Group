/*Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.*/
//Define L298N Dual H-Bridge Motor Controller Pins
#define dir1PinL  2    //Motor direction
#define dir2PinL  4    //Motor direction
#define speedPinL 6    // Needs to be a PWM pin to be able to control motor speed

#define dir1PinR  7    //Motor direction
#define dir2PinR  8   //Motor direction
#define speedPinR 5   // Needs to be a PWM pin to be able to control motor speed

/*From left to right, connect to D3,A1-A3 ,D10*/
#define LFSensor_0 11  
#define LFSensor_1 A1
#define LFSensor_2 A2
#define LFSensor_3 A3
#define LFSensor_4 10

//#define OptechPin 3

#define M_SPEED1   240  //motor speed TO AJUST RIGHT MOTOR 200    -1
#define M_SPEED2   250  //motor speed 230  255
#define M_SPEED3   238  //MOTOR SPEEND TO AJUEST LEFT MOTOTER
