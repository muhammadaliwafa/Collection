#include <ArduinoJson.h>
#include <SoftwareSerial.h>

SoftwareSerial toESP(3, 2);
void setup() {
  Serial.begin(9600);
  toESP.begin(9600);
  String input =
      "{\"sensor1\":\"gps\",\"sensor2\":1351824120,\"float\":3.786,\"data\":[48.756080,2.302038]}";
  toESP.println(input); //mengirim data ke ESP melalui serial
}

void loop() {

  }
