#include <ArduinoJson.h>
#include <SoftwareSerial.h>
SoftwareSerial fromArduino(D2,D3);//(RX, TX ESP)
 String dataKirim;

 
void setup() {
  Serial.begin(9600);
  fromArduino.begin(9600);

}

void loop() {
  if(fromArduino.available())
  {
    char  c = fromArduino.read();
    if(c == '}')
    {
      dataKirim += c; //data masuk
      String input = dataKirim;
      float v = value(input, "float").toFloat();
      long it = value(input, "sensor2").toInt();
      String str = value(input, "sensor1");
      Serial.println(v,3);
      Serial.println(it);
      Serial.println(str);
      dataKirim = "";
    }
    else
    {
      dataKirim += c;
    } 
  }
}

String value(String input, String key){
  int sindex = input.indexOf(key);
  int koma = input.indexOf(",", sindex);
  int colon = input.indexOf(":", sindex);
  String value = input.substring(colon+1, koma);
  return value;
}
