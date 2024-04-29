
#include <TM1637Display.h> //included library for TM1637 Display
 
//global variables
 
#define trig1Pin 10  //pin definition for trig1                                                                        
#define echo1Pin 11  //pin definition for echo1                                                                          
#define trig2Pin 2   //pin definition for trig2                                                                           
#define echo2Pin 3   //pin definition for echo2                                                                           
 
#define CLK 4    //pin definition for CLK 
#define DIO 5    //pin definition for DIO 
TM1637Display display(CLK,DIO);  //initialization of display pins
 
int counter = 0; //setting persons to 0                
 
void setup() {
    Serial.begin (9600);  //Serial Monitor is set to 9600 Bauds                                                                         
    pinMode(trig1Pin, OUTPUT);  //trig1 is defined as an output                                                                   
    pinMode(echo1Pin, INPUT);   //echo1 is defined as input
    pinMode(trig2Pin, OUTPUT);  //trig2 is defined as an output
    pinMode(echo2Pin, INPUT);   //echo2 is defined as an input
    
    display.setBrightness(0x0f); //setting the light to max
    uint8_t data[]={0x00,0x00,0x00,0xff}; //to turn off the screen
    display.setSegments(data);   //setting the value of each digit
}
 
double Sensor1(){
 
  digitalWrite(trig1Pin, LOW);   //clears the trig1Pin
  delayMicroseconds(2); 
  digitalWrite(trig1Pin, HIGH); //sensor 1 is activated
  delayMicroseconds(10);        
  digitalWrite(trig1Pin, LOW); //sensor 1 is turned off
 
  double duration_sensor1 = pulseIn(echo1Pin, HIGH); 
  double distance_sensor1 = duration_sensor1 * 0.034 / 2; //distance calculation
 
 
  return distance_sensor1;
}
 
double Sensor2(){
 
  digitalWrite(trig2Pin, LOW);   //clears the trig2Pin
  delayMicroseconds(2);
  digitalWrite(trig2Pin, HIGH); //sensor 2 is activated
  delayMicroseconds(10);        //wait 1ms
  digitalWrite(trig2Pin, LOW); //sensor 2 is turned off
 
  double duration_sensor2 = pulseIn(echo2Pin, HIGH); //distance calculation
  double distance_sensor2 = duration_sensor2 * 0.034 / 2;
 
 
  return distance_sensor2;
}
 
int curr_time=0;
//double diff=0;
double sensor1_distance, sensor2_distance;
 
void loop() {
 
  display.showNumberDec(counter,false,3,1); //the result is printed on the led display
 
  sensor1_distance = Sensor1();
  sensor2_distance = Sensor2();
 
  //diff = sensor1_distance - sensor2_distance;
 
  if(sensor1_distance){
    if (sensor1_distance <= 30) //if the distance is less than or equal to 30 cm 
    {
        unsigned long curr_time = millis();      
        bool flag = false;
        while(millis() - curr_time < 3000){  //until 3 seconds have passed
          sensor2_distance = Sensor2();
 
          if(sensor2_distance) 
          {
          if (sensor2_distance <= 30) //if the distance is less than or equal to 30 cm
        { 
                  counter++;    //increase by 1 every time
                  flag = true;
                  display.showNumberDec(counter,false,3,1); 
                  delay(1000); 
              }
      
            if (flag == true)
                  return;
        }
      }
    }
  }
  delay(100);
  if(sensor2_distance){
      if(sensor2_distance <= 30) 
      { 
          unsigned long curr_time = millis();
          bool flag = false;
          while(millis() - curr_time < 3000){  //until 3 seconds have passed
 
            sensor1_distance = Sensor1();
        if(sensor1_distance)
        {
              if( sensor1_distance <= 30 ) //if the distance is less than or equal to 30 cm
              {      
                  counter--; //decrease by 1 every time
                  if (counter<0)counter=0; //if it goes negative, return it to zero
                  flag = true;
                  display.showNumberDec(counter,false,3,1); 
                  delay(1000); //wait for the person to move out of range
              }
              if (flag == true)
                  return;
          }
      }
    }
  }
 //delay(500);
}
