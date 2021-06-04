void setup() {
  Serial.begin(9600);
  DDRC &= B00000000; //mengubah pin 30-37 menjadi input
  PORTC = B11111111;
  DDRA &= B00000000;
  PORTA = B11111111;
}

void loop() {
  uint16_t b;
  b = ((b|PINA) << 8)|PINC;
  for (int i=15; i>=0; i--){
    Serial.print(bitRead(b,i));
  }
  Serial.println();

}
