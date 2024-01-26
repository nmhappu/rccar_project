#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

#define enA 2
#define in1 4
#define in2 5
#define enB 3
#define in3 6
#define in4 7
#define A4_PIN 18

Servo myservo;
RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";
int s_state=0;

int pos = 0;
int xAxis;
int yAxis;
int swit;
int l_stop = 0;
int r_stop = 0;

unsigned long previousMillis = 0;
const int interval = 40;

void forward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void backward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void left() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void right() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void forward_l() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void forward_r() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void start() {
}

void stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void servo1() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (s_state==0) {
      myservo.write(pos);
      pos += 1;
      if(pos==135){
        s_state=1;
      }
    } else if (s_state==1) {
      
      pos-=1;
     myservo.write(pos);
     if(pos==45)
     {
      s_state=0;
     }


      // If you want to perform additional actions after reaching 180 degrees,
      // add the code here.
    }
  }
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
  pinMode(A4_PIN, OUTPUT);
  digitalWrite(A4_PIN, HIGH);
  myservo.attach(10);

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char xyData[32] = "";
    radio.read(&xyData, sizeof(xyData));

    String dataReceived = String(xyData);
    int commaIndex1 = dataReceived.indexOf(',');
    int commaIndex2 = dataReceived.indexOf(',', commaIndex1 + 1);

    xAxis = dataReceived.substring(0, commaIndex1).toInt();
    yAxis = dataReceived.substring(commaIndex1 + 1, commaIndex2).toInt();
    swit = dataReceived.substring(commaIndex2 + 1).toInt();

    Serial.print("X: ");
    Serial.print(xAxis);
    Serial.print(" Y: ");
    Serial.print(yAxis);
    Serial.print(" Switch: ");
    Serial.println(swit);
  }

  l_stop = analogRead(A2);
  r_stop = analogRead(A3);

  if (swit == 0) {
   digitalWrite(A4_PIN, LOW);
   servo1();
    Serial.println("Switch Pressed ********");
    
  }
  else{
     digitalWrite(A4_PIN, HIGH);
  }

  if ((yAxis > 900) && ((xAxis < 900) && (xAxis > 200)) && (l_stop > 100) ) {
    Serial.println("Forward");
    forward();
  } else if ((yAxis < 200) && ((xAxis < 900) && (xAxis > 200))&& (r_stop > 100)) {
    Serial.println("Backward");
    backward();
  } else if ((xAxis < 200) && ((yAxis > 200) && (yAxis < 900))) {
    Serial.println("Left");
    left();
  } else if ((xAxis > 900) && ((yAxis > 200) && (yAxis < 900))) {
    Serial.println("Right");
    right();
  } else if ((xAxis < 200) && (yAxis > 900)) {
    Serial.println("F-Left");
    forward_l();
  } else if ((xAxis > 900) && (yAxis > 900)) {
    Serial.println("F-Right");
    forward_r();
  } else {
    stop();
  }
}
