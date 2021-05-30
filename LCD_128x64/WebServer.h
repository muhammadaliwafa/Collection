
// WIFI

// LED Internal
//uint8_t pin_led = 2;

//WEB Server
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

// Sebagai Station
const char* wifissid = "all"; //kalau gagal konek
const char* wifipassword = "qwerty234";

// Sebagai AccessPoint
const char* ssid = "JWS Al-Furqon"; //kalau gagal konek
const char* password = "alfurqon";

IPAddress local_ip(192, 168, 4, 1);
IPAddress gateway(192, 168, 4, 1);
IPAddress netmask(255, 255, 255, 0);



void wifiConnect() {

  WiFi.softAPdisconnect(true);
  WiFi.disconnect();
  delay(1000);

  Serial.println("Mencoba sambungan ke Hotspot atau Router");
  WiFi.mode(WIFI_STA);
//  WiFi.begin(configjws.ssid, configjws.password);
  WiFi.begin(wifissid, wifipassword);
  Serial.println(configjws.ssid);
  Serial.println(configjws.password);
  
  unsigned long startTime = millis();
  
  while (WiFi.status() != WL_CONNECTED) {
    
    delay(500);
    Serial.print(".");
    
//    digitalWrite(pin_led, !digitalRead(pin_led));
    
    if (millis() - startTime > 5000) {
      Serial.println(" ");
      break;
    }
    
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    
//    digitalWrite(pin_led, HIGH);
    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    
  } else {
    
    Serial.println("Gagal tersambung ke Hotspot, mode Hotspot aktif.");
    
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(local_ip, gateway, netmask);
    WiFi.softAP(configjws.ssid, configjws.password);
//    WiFi.softAP(ssid, password);
    
//    digitalWrite(pin_led, LOW);

    Serial.print("MAC: ");
    Serial.println(WiFi.macAddress());
    Serial.print("IP: ");
    Serial.println(local_ip);
    
  }

}



void handleXMLWaktu(){
  String Json = XMLWaktu();
  server.send(200,"text/xml",Json);
//  XMLWaktu();
//  server.send(200,"text/xml",XML);
}

void handleXMLDataJWS(){

  String Json = XMLDataJWS();
  server.send(200,"text/xml", Json);
//  XMLDataJWS();
//  server.send(200,"text/xml",XML);
}

void handleSelaraskan(){
  String dataselaraskan = server.arg("plain");

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataselaraskan);
  uint8_t jam = doc["jam"];
  uint8_t menit = doc["menit"];
  uint8_t detik = doc["detik"];
  uint8_t hari = doc["hari"];
  uint8_t tgl = doc["tgl"];
  uint8_t bulan = doc["bulan"];
  uint16_t tahun = doc["tahun"];

  RtcDateTime now = Rtc.GetDateTime();
  Rtc.SetDateTime(RtcDateTime(tahun, bulan, tgl, jam, menit, detik));

  Serial.println(detik);
  String response = "{\"status\":";
  response += 1;
  response +="}";
  server.send(200, "/selaraskan", response);

  BacaRTC(); 
  updateJWS();
  TanggalHijriah();
  bacaParameter();
  
}

void handleSettingJwsUpdate() {

  String datajws = server.arg("plain");

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, datajws);

  File configFileJws = LittleFS.open(fileconfigjws, "w");
  
  if (!configFileJws) {
    Serial.println("Gagal membuka Info configFileJws untuk ditulis");
    return;
  }
  
  serializeJson(doc, configFileJws);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileJws.close();
    Serial.println("Berhasil mengubah configFileJws");

    loadJwsConfig(fileconfigjws, configjws);
  
  } 
  BacaRTC(); 
  updateJWS();
  TanggalHijriah();
  bacaParameter();

}

//void handleKecerahan(){
//  String dataselaraskan = server.arg("plain");
//  Serial.println("data adalah :" + dataselaraskan);
//  DynamicJsonDocument doc(1024);
//  DeserializationError error = deserializeJson(doc, dataselaraskan);
//
//  File configFileDisp = LittleFS.open(fileconfigdisp, "w");
//  if (!configFileDisp) {
//    Serial.println("Gagal membuka Display configFile untuk ditulis");
//    return;
//  }
//
//  serializeJson(doc, configFileDisp);
//
//  if (error) {
//    
//    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
//    Serial.println(error.c_str());
//    return;
//    
//  } else {
//    
//    configFileDisp.close();
//    Serial.println("Berhasil mengubah configDispJws");
//
//    loadDispConfig(fileconfigdisp, configdisp);
////    bacaParameter();
//  
//  } 
//
//  
//
//  String response = "{\"status\":";
//  response += 1;
//  response +="}";
//  server.send(200, "/kecerahan", response);
//}
