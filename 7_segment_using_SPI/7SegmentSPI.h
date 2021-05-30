#define OUTPUT_ENABLE_PIN 9
#define SS_PIN 10

int angkaLSB[10]{
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

int angkaMSB[10]{
  0xC0, //0
  0xF9, //1
  0xA4, //2
  0xB0, //3
  0x99, //4
  0x92, //5
  0x82, //6
  0xF8, //7
  0x80, //8
  0x90 //9
};


byte digits[4]= {1, 1, 1, 1};
byte digit;
void tampil(){
  digitalWrite(SS_PIN, LOW);
  for( digit = 0; digit<4; digit++){
    SPI.transfer(angkaMSB[digits[digit]]);
    }
  digitalWrite(SS_PIN, HIGH);  
}


void RTC2digit(){
  static uint8_t lastDigit;
  uint8_t k = rJam;
  int digit1=k/10;
  digits[0]=digit1;
  int digit2 = k - (digit1*10);
  digits[1]=digit2;
  k = rMen;
  digit1=k/10;
  digits[2]=digit1;
  digit2 = k - (digit1*10);
  digits[3]=digit2;
  if(lastDigit!=digits[3]){
    tampil();
    lastDigit=digits[3];
    Serial.println(rHar);
  }
  
}
