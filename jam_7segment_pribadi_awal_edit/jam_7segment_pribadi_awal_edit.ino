#include <TimerOne.h>

const int DATA_PIN = 11;
const int LATCH_PIN = 10;
const int CLOCK_PIN = 13;
const int OUTPUT_ENABLE_PIN = 9;


int angka[10]{
  0x03, //0
  0x9F, //1
  0x25, //2
  0x0D, //3
  0x99, //4
  0x49, //5
  0x41, //6
  0x1F, //7
  0x01, //8
  0x09 //9
};


byte digits[4]= {1, 1, 1, 1};
byte digit;
int j;

void setup() {

  
  Serial.begin(9600);
 pinMode(DATA_PIN, OUTPUT);
 pinMode(LATCH_PIN, OUTPUT);
 pinMode(CLOCK_PIN, OUTPUT);
 pinMode(12, OUTPUT);
// pinMode(10, OUTPUT);
 pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
 pinMode(11, OUTPUT);
 analogWrite(OUTPUT_ENABLE_PIN, 200);
 tampil();
 pinMode(4, OUTPUT);
 digitalWrite(4, HIGH);

  int a = 0xC0;
  for(int i=0; i<8; i++){
    bool b = bitRead(a, i);
    Serial.println(b);
  }
}

void loop() {

  for(byte k=59; k>0; k--){
    int digit3=k/10;
    digits[2]=digit3;
    int digit4 = k - (digit3*10);
    digits[3]=digit4;
    tampil();
    delay(1000);
  }
  
  

}


void tampil(){
  digitalWrite(LATCH_PIN, LOW);
  for( digit = 0; digit<4; digit++){
      for( j=0; j<8; j++){
        bool b = bitRead(angka[digits[digit]], j);
        digitalWrite(CLOCK_PIN, LOW);
        digitalWrite(DATA_PIN, b);
        digitalWrite(CLOCK_PIN, HIGH);
      }
    }
     digitalWrite(LATCH_PIN, HIGH);  
}
