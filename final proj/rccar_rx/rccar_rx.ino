/*
   Arduino Robot Car Wireless Control using the NRF24L01 Transceiver module

             == Receiver - Arduino robot car ==

   by Dejan Nedelkovski, www.HowToMechatronics.com

   Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define enA 2   // Note: Pin 9 in previous video ( pin 10 is used for the SPI communication of the NRF24L01)
#define in1 4
#define in2 5
#define enB 3   // Note:  Pin 10 in previous video
#define in3 6
#define in4 7

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

char receivedData[32] = "";
int  xAxis, yAxis,swit="0";

int l_stop= 0;
int r_stop = 0;
void forward()
{
    digitalWrite(in1, LOW);//left forwaard
    digitalWrite(in2, HIGH);

    digitalWrite(in3, LOW);//right forward
    digitalWrite(in4, HIGH);
}
void backword(){
    digitalWrite(in1, HIGH);//left backword
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, HIGH);//left backword
    digitalWrite(in4, LOW);
}
void left(){
    digitalWrite(in1, LOW);//LEFT STOP
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, LOW);//RIGHT FORWARD
    digitalWrite(in4, HIGH);
}
void right(){
    digitalWrite(in1, LOW);//LEFT FORWARD
    digitalWrite(in2, HIGH);
    
    digitalWrite(in3, LOW);//RIGHT STOP
    digitalWrite(in4, LOW);
}
void forward_l()
{
    digitalWrite(in1, HIGH);//LEFT BACKWORB
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, LOW);//RIGHT FORWARD
    digitalWrite(in4, HIGH);
}

void forward_r(){
    digitalWrite(in1, LOW);//LEFT FORWARD
    digitalWrite(in2, HIGH);
    
    digitalWrite(in3, HIGH);//RIGHT FORWARD
    digitalWrite(in4, LOW);
}

void start(){
   
}
void stop(){
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
}

void setup() {
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  digitalWrite(enA, HIGH);
  digitalWrite(enB, HIGH);
  
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

}

void loop() {

  if (radio.available()) {   // If the NRF240L01 module received data

    radio.read(&receivedData, sizeof(receivedData)); // Read the data and put it into a character array
    xAxis = atoi(&receivedData[0]); // Convert the data from the character array (received X value) into an integer
    delay(10);

    radio.read(&receivedData, sizeof(receivedData));
    yAxis = atoi(&receivedData[0]);
    delay(10);

  }

  l_stop=analogRead(A2);
  r_stop=analogRead(A3);
  Serial.println(swit);
 
  if ((yAxis > 900)&&((xAxis < 900)&&(xAxis > 200))&&(l_stop>100)) {
    Serial.println("Forward");
    forward();
    }

  else if ((yAxis < 200)&&((xAxis < 900)&&(xAxis > 200))) {
    Serial.println("Backward");
    backword();
    }

  else if ((xAxis < 200)&&((yAxis > 200)&&(yAxis < 900))) {
    Serial.println("Left");
    left();
    }

  else if ((xAxis > 900)&&((yAxis > 200)&&(yAxis < 900))) {
    Serial.println("Right");
    right();
    }

// Diagonals ============

  else if ((xAxis < 200)&& (yAxis > 900)){
    Serial.println("F-Left");
    forward_l();
    }

  else if ((xAxis > 900)&& (yAxis > 900)){
    Serial.println("F-Right");
    forward_r();
    }

  else {
    //Serial.println("B-Left");
    stop();
    }

//  analogWrite(enA, motorSpeedA); // Send PWM signal to motor A
// analogWrite(enB, motorSpeedB); // Send PWM signal to motor B
}
