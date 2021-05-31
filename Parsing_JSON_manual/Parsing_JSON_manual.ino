void setup() {
  Serial.begin(9600);
  String input =
      "{\"sensor1\":\"gps\",\"sensor2\":1351824120,\"float\":3.786,\"data\":[48.756080,2.302038]}";
  
  float v = value(input, "float").toFloat();
  long it = value(input, "sensor2").toInt();
  String str = value(input, "sensor1");
  Serial.println(v,3);
  Serial.println(it);
  Serial.println(str);



}

String value(String input, String key){
  int sindex = input.indexOf(key);
  int koma = input.indexOf(",", sindex);
  int colon = input.indexOf(":", sindex);
  String value = input.substring(colon+1, koma);
  return value;
}

void loop() {
  // not used in this example
}
