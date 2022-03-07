#include <Pixy2.h>

#define sensor1 A0
#define sensor2 A1

#define Enable1 6 // enable line for motor 1
#define Input1  7// digitial line for motor 1
#define Input2  8// digitial line for motor 1

#define Enable2 5 // enable line for motor 2
#define Input3 4 // digitial line for motor 2
#define Input4 2 // digitial line for motor 2

int LastColor=100; // initialize the color code

int volts1=0;
int volts2=0;

Pixy2 pixy;// create a pixy object
int signal=9;// assgin pin to the PING sensor
int distance;
int T_90 = 800;
int T_180 = 1500;
int Back = 200;
unsigned long pulseduration=0;

// optimum working voltage = 7.48

void setup() {

  Serial.begin(9600);
  
  //We need to initialize the pixy object 
  pixy.init();
  
  // Use color connected components program for the pan tilt to track 
  pixy.changeProg("color_connected_components");
  
  // setting to the motor 1
  pinMode(Enable1,OUTPUT);
  pinMode(Input1,OUTPUT);
  pinMode(Input2,OUTPUT);
    
  // setting to the motor 2
  pinMode(Enable2,OUTPUT);
  pinMode(Input3,OUTPUT);
  pinMode(Input4,OUTPUT);
  
  // setting to the ping sensor
  pinMode(signal, OUTPUT);

  //Monitor Interrupt
  //attachInterrupt(digitalPinToInterrupt(signal), measureDistance, HIGH);
}



// measurement of the distance by the ping sensor
void measureDistance()
{
 Serial.println("PING sensor working in progress");
 // set pin as output so we can send a pulse
 pinMode(signal, OUTPUT);
// set output to LOW
 digitalWrite(signal, LOW);
 delayMicroseconds(5);
 
 // now send the 5uS pulse out to activate Ping)))
 digitalWrite(signal, HIGH);
 delayMicroseconds(5);
 digitalWrite(signal, LOW);
 
 // now we need to change the digital pin
 // to input to read the incoming pulse
 pinMode(signal, INPUT);
 
 // finally, measure the length of the incoming pulse
 pulseduration=pulseIn(signal, HIGH);
 
 // Calculating distance
 distance = ((343.0/10000.0*(pulseduration)))/2.0;
}



void loop() {

// measure the side distance by IR sensors
  
  
  volts1 = analogRead(sensor1);
  volts2 = analogRead(sensor2);
 
  IR();

// measure the distance by ping 
  measureDistance();
  delay(25);
  
  measureDistance(); 

if(distance<4.5){
  moveBack();
  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(400);
}

//get objects stored in the pixy camera 
  pixy.ccc.getBlocks();
  
if(pixy.ccc.blocks[0].m_signature==3||pixy.ccc.blocks[0].m_signature==1||pixy.ccc.blocks[0].m_signature==2){
  LastColor = pixy.ccc.blocks[0].m_signature;
}
//purple: 2 || green: 1 || orange: 3


if(distance<10.0 && LastColor==3){
  LastColor = 100;
  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(3000); 
  turnLeft();
  IR();
}

else if(distance<10.0 && LastColor==1){
  LastColor = 100;
  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(3000); 
  turnRight();
  IR();
}

else if(distance<10.0 && LastColor==2){
  LastColor = 100;
  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(3000); 
  turnAround();
  IR();
}

else {
  IR();
  //Serial.println("NO Cue Detected: Move forward");
  moveForward();  
}
 
}

void turnRight(){
  // run motor 1
  //Serial.println("moving clockwise: engine 1");
  analogWrite(Enable1,115); 
  digitalWrite(Input1,HIGH);
  digitalWrite(Input2,LOW);
  Serial.print("here");
  // run motor 2
  //Serial.println("moving counter-clockwise: engine 2");
  analogWrite(Enable2,113); 
  digitalWrite(Input3,LOW);
  digitalWrite(Input4,HIGH);

  delay(T_90); // operation time

  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(1500);
  
 }

 void turnLeft(){
  // run motor 1
  //Serial.println("moving counter-clockwise: engine 1");
  analogWrite(Enable1,115); 
  digitalWrite(Input1,LOW);
  digitalWrite(Input2,HIGH);
  
  // run motor 2
  //Serial.println("moving clockwise: engine 2");
  analogWrite(Enable2,113); 
  digitalWrite(Input3,HIGH);
  digitalWrite(Input4,LOW);

  delay(T_90); // operation time

  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(1500);
 }

 void turnAround(){
    // run motor 1
  //Serial.println("moving counter-clockwise: engine 1");
  analogWrite(Enable1,115); 
  digitalWrite(Input1,HIGH);
  digitalWrite(Input2,LOW);
  
  // run motor 2
  //Serial.println("moving clockwise: engine 2");
  analogWrite(Enable2,113); 
  digitalWrite(Input3,LOW);
  digitalWrite(Input4,HIGH);

  delay(T_180); // operation time

  analogWrite(Enable1,0);
  analogWrite(Enable2,0);
  delay(1500);
  }

 void moveForward(){
  // run motor 1
  //Serial.println("moving counter-clockwise: engine 1");
  analogWrite(Enable1,115); 
  digitalWrite(Input1,LOW);
  digitalWrite(Input2,HIGH);
  IR();
  // run motor 2
  //Serial.println("moving counter-clockwise: engine 2");
  analogWrite(Enable2,113); 
  digitalWrite(Input3,LOW);
  digitalWrite(Input4,HIGH);
  IR();
 }

 void moveBack(){
  analogWrite(Enable1,115); 
  digitalWrite(Input1,HIGH);
  digitalWrite(Input2,LOW);
  
  // run motor 2
  //Serial.println("moving counter-clockwise: engine 2");
  analogWrite(Enable2,113); 
  digitalWrite(Input3,HIGH);
  digitalWrite(Input4,LOW);
  delay(Back); 
  
 }

 void IR(){
    if(volts1>540){//right IR sensor  
    //Serial.print("IRright");
    //turn left
    analogWrite(Enable1,115); 
    digitalWrite(Input1,LOW);
    digitalWrite(Input2,HIGH);
  
    analogWrite(Enable2,113); 
    digitalWrite(Input3,HIGH);
    digitalWrite(Input4,LOW);
  
    delay(25); // operation time
  }

  if(volts2>540){//left IR sensor
    //Serial.print("left");
    //turn right
    analogWrite(Enable1,115); 
    digitalWrite(Input1,HIGH);
    digitalWrite(Input2,LOW);
  
    analogWrite(Enable2,113); 
    digitalWrite(Input3,LOW);
    digitalWrite(Input4,HIGH);
  
    delay(25); // operation time
  }
 }

 
