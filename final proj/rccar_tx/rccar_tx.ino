#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9); // CE, CSN
const byte address[6] = "00001";

int xAxisValue, yAxisValue;
String swit;

void setup() {
  Serial.begin(9600);
  radio.begin();
  pinMode(2, INPUT);
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void loop() {
  xAxisValue = analogRead(A0); // Read Joystick X-axis
  yAxisValue = analogRead(A1); // Read Joystick Y-axis

  swit = digitalRead(2) == HIGH ? "1" : "0"; // Read the switch state

  // Create a formatted string for sending data
  String dataToSend = String(xAxisValue) + "," + String(yAxisValue) + "," + swit;
  
  // Convert the String to a char array
  char xyData[dataToSend.length() + 1];
  dataToSend.toCharArray(xyData, sizeof(xyData));

  // Send the data to the other NRF24L01 module
  radio.write(&xyData, sizeof(xyData));

  Serial.print("X: "); Serial.print(xAxisValue);
  Serial.print(" Y: "); Serial.print(yAxisValue);
  Serial.print(" Switch: "); Serial.println(swit);
  
  delay(20);
}
