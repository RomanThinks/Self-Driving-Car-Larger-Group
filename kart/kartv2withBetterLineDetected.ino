#include "configuration.h"

#include "Adafruit_TCS34725.h"
#include <Wire.h>

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X); 

 const byte OptechPin = 3 ;
 int sensor[5];

 const int buzzer = 13;     //buzzer digital pin
 
 /*read line follow sensors*/
void read_sensor_values()
{
  sensor[0]=digitalRead(LFSensor_0);
  sensor[1]=digitalRead(LFSensor_1);
  sensor[2]=digitalRead(LFSensor_2);
  sensor[3]=digitalRead(LFSensor_3);
  sensor[4]=digitalRead(LFSensor_4);
}


//Set the direction as well enable the speed of the Motores
void go_Advance(void)  //Forward
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,HIGH);
  digitalWrite(dir2PinR,LOW);
  set_Motorspeed(M_SPEED3,M_SPEED1);
}
void go_Left(void)  //Turn left
{
  digitalWrite(dir1PinL, HIGH);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,HIGH);
  set_Motorspeed(0,M_SPEED3);
}
void go_Right(void)  //Turn right
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
  digitalWrite(dir1PinR,HIGH);
  digitalWrite(dir2PinR,LOW);
  set_Motorspeed(M_SPEED1,0);
}
void go_Back(void)  //Reverse
{
  digitalWrite(dir1PinL, LOW);
  digitalWrite(dir2PinL,HIGH);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,HIGH);
}
void stop_Stop()    //Stop
{
  digitalWrite(dir1PinL,LOW);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,LOW);
  digitalWrite(buzzer, HIGH);     //Turns off Buzzer
}
/*set motor speed */
void set_Motorspeed(int speed_L,int speed_R)
{
  analogWrite(speedPinL,speed_L);
  analogWrite(speedPinR,speed_R);  
}

void isr1() {
  digitalWrite(dir1PinL,LOW);
  digitalWrite(dir2PinL,LOW);
  digitalWrite(dir1PinR,LOW);
  digitalWrite(dir2PinR,LOW);

  tone(buzzer, 5000);         //Buzzer to trigger every time the interrupt is called
  delay(1000);      
  noTone(buzzer);    
  delay(1000);      
  
  stop_Stop();                
}
 
void setup()
{
 //  Opesetup: 
 pinMode(OptechPin, INPUT);
 attachInterrupt(digitalPinToInterrupt(OptechPin), isr1, LOW);

 //Buzzer setup
 pinMode(buzzer, INPUT);
 
 //motors
 pinMode(speedPinL,OUTPUT); //left motor PWM pin
 pinMode(speedPinR,OUTPUT); //rignt motor PWM  pin
 pinMode(dir1PinL,OUTPUT); //left motor direction pin1
 pinMode(dir2PinL,OUTPUT); //left motor direction pin2
 pinMode(dir1PinR,OUTPUT); //right motor direction Pin 1
 pinMode(dir2PinR,OUTPUT);  //right motor direction Pin 2

  /*line follow sensors */
 pinMode(LFSensor_0,INPUT);
 pinMode(LFSensor_1,INPUT);
 pinMode(LFSensor_2,INPUT);
 pinMode(LFSensor_3,INPUT);
 pinMode(LFSensor_4,INPUT);
 Serial.begin(9600);
}

void auto_tarcking(){
  read_sensor_values();
  //Checks if black line is being detected at all if not set it go backward. 
  if(sensor[0]==LOW && sensor[1]==LOW && sensor[2]==LOW && sensor[3]==LOW && sensor[4]==LOW)  {
     go_Back();
     set_Motorspeed(M_SPEED1,M_SPEED1);
     //Delay is for how long should go backward. 
     delay(200);
  }
  //IF the sensor detected the black line in the middle then go forward
  else if(sensor[2]==LOW){ //The middle sensor is on the black line
    if(sensor[1]==LOW && sensor[3]==LOW) { //Other sensors are in the white area
      go_Advance();  //straight
      
    }
    //If not the left sensor is also on the black line 
    else if(sensor[1]==HIGH && sensor[3]==LOW){ //The second sensor is also on the black line
       go_Left();  //Turn left
       
       
    }
    //if not the right sensor is also on the black line 
    else if(sensor[1]==LOW && sensor[3]==HIGH){  //The fourth sensor is also on the black line
      go_Right();  //Turn right
      
    }
  }
  else if(sensor[2]==LOW)
  {  //The middle sensor is in the white area
    if(sensor[1]==HIGH && sensor[3]==LOW){ //Turn left
      go_Left();  
    
    }
    else if(sensor[1]==LOW && sensor[3]==HIGH){  //Turn right
       go_Right();
       
    }
  }
  
  else if(sensor[1]==HIGH){  //The second sensor is on the black line
    if(sensor[0]==HIGH && sensor[2]==LOW){ //Accelerate left turn
      go_Left();
    }
    else{  //Turn left
      go_Left();
    }
  }
  else if(sensor[3]==HIGH){  //The fourth sensor is on the black line
    if(sensor[2]==LOW && sensor[4]==HIGH){  //Accelerate right turn
      go_Right();
    }
    else{  //Turn right
      go_Right();
    }
  }
}


// Reads color code and stops car if detects RED.
void colorData() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  //Printing for testing purposes:
  //Serial.print("R: "); Serial.print(r, DEC); Serial.print(" ");
  //Serial.print("G: "); Serial.print(g, DEC); Serial.print(" ");
  //Serial.print("B: "); Serial.print(b, DEC); Serial.println(" ");
  //Serial.print("C: "); Serial.print(c, DEC); Serial.print(" ");
  //Serial.println(" ");

  if (r > 600 && g<500 && b<500) { //if red is detected at all, stop for 3 seconds
    stop_Stop();
    delay(3000);
  }
}

void loop(){
    
    colorData();
    auto_tarcking();
 
}
