#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
int lsstate = 1;
void setup() {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(10, INPUT_PULLUP);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00002
  radio.openReadingPipe(1, addresses[0]); // 00001
  radio.setPALevel(RF24_PA_MIN);
}
void loop() {
  static int angle;
  static int cmd;
  delay(5);
  radio.stopListening();
  int potValue = digitalRead(10);
//  int angleValue = map(potValue, 0, 1023, 0, 180);
  if(potValue){
    cmd = 180;
  }
  else{
    cmd =1;
  }
  radio.write(&cmd, sizeof(cmd));
  delay(5);
  radio.startListening();
//  while (!radio.available()){
//    Serial.println("hh");
//    digitalWrite(9, HIGH);
//  }
//  else{
//    digitalWrite(9, LOW);
//  }
  int data;
  radio.read(&data, sizeof(data));
  if(data>0){
    angle = data;
  }
  Serial.println(angle);
  if (angle == 1) {
    digitalWrite(9, HIGH);
    Serial.println("hidup");
  }
  else if(angle==2){
    digitalWrite(9, LOW);
    Serial.println("Mati");
  }

  if(lsstate!=angle){
    lsstate = angle;
    digitalWrite(6, HIGH);
    delay(200);
    digitalWrite(6, LOW);
  }
}
