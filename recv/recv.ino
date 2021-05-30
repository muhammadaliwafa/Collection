#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
Servo myServo;
boolean buttonState = 0;
int angle = 180;
int lastangle = 90;
int menit = 0;
void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, LOW);
  pinMode(10, OUTPUT);
  myServo.attach(9);
  radio.begin();
  radio.openWritingPipe(addresses[0]); // 00001
  radio.openReadingPipe(1, addresses[1]); // 00002
  radio.setPALevel(RF24_PA_MIN);
}

void(* resetFunc) (void) = 0; //declare reset function @ address 0
void loop() { 
  static int cmd;
  static uint32_t lsRn;
  static int lscmd;
  uint32_t tmr = millis();
  delay(5);
  radio.startListening();
  if ( radio.available()) {
    digitalWrite(10, HIGH);
    lsRn = tmr;
    while (radio.available()) {
      int data;
      radio.read(&data, sizeof(data));
      if(data>=0){
        angle = data;
        lastangle = angle;
      }
      
    }
    delay(50);
    radio.stopListening();
    uint8_t anglesent;
    if(digitalRead(2)){
      anglesent = 1;
    }
    else{
      anglesent = 2;
    }
    radio.write(&anglesent, sizeof(anglesent));
  }
  else{
    digitalWrite(10, LOW);
  
    //  Serial.println(digitalRead(2));
    if((tmr-lsRn)<0){
      lsRn = tmr;
    }
    else if((tmr-lsRn)>60000){
      lsRn=tmr;
      angle = 180;
      menit++;
    }
    if(menit>5){
      menit=0;
      Serial.println("reset");
      delay(1000);
      resetFunc();  //call reset
    }
  }
  myServo.write(angle);
}
