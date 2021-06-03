#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "all"
#define WIFI_PASSWORD "qwerty234"

#define FIREBASE_HOST "aulia-ta-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "wt7PBdWfSVZol9lxGL0FWBDv29eMh6WzrrgQcQSx"

FirebaseData fbdo;
FirebaseData fbdo1;
FirebaseData fbdo2;

FirebaseJsonArray arr;

void printResult(FirebaseData &data);

unsigned long sendDataPrevMillis = 0;

String path = "/Data/Array";
int power = 0;

uint16_t count = 0;
uint16_t warna = 3, kecepatan=3, counter=0;
String resi[] = {"1","2","3","4","5","6","7","8","9","0"};

void setup()
{

    Serial.begin(115200);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(300);
    }
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);

    //Set the size of WiFi rx/tx buffers in the case where we want to work with large data.
    fbdo.setBSSLBufferSize(1024, 1024);
    fbdo1.setBSSLBufferSize(1024, 1024);
    fbdo2.setBSSLBufferSize(1024, 1024);

    //Set the size of HTTP response buffers in the case where we want to work with large data.
    fbdo.setResponseSize(1024);
    fbdo1.setResponseSize(1024);
    fbdo2.setResponseSize(1024);

    if (!Firebase.beginStream(fbdo, path))
    {
        Serial.println("------------------------------------");
        Serial.println("Can't begin stream connection...");
        Serial.println("REASON: " + fbdo.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
    }
    Serial.println("setup selesai");
    pinMode(2, OUTPUT);
}

void loop() {

  if (!Firebase.readStream(fbdo))
    {
        Serial.println("\n Can't read stream data...");
        Serial.println("\n REASON: " + fbdo.errorReason());
    }

    if (fbdo.streamTimeout())
    {
        Serial.println("Stream timeout, resume streaming...\n");
    }
    if (fbdo.streamAvailable())
    {
//        Serial.println("****** \nStream Data available...");
        printResult(fbdo, false);
        Serial.println("Get Array...");
        if (Firebase.get(fbdo1, path + "/Perintah"))
        {
            Serial.println("PASSED");
//            Serial.println("PATH: " + fbdo1.dataPath());
//            Serial.println("TYPE: " + fbdo1.dataType());
//            if(fbdo1.dataType()=="null"){
//              Serial.println("KOSONG");
//              buatData(fbdo1);
//            }
            Serial.print("VALUE: ");
            printResult(fbdo1, true);
            Serial.println("------------------------------------");
//            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo1.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
    }


    
    if(power==1){
      digitalWrite(2, LOW);
    }else digitalWrite(2, HIGH);


    static uint16_t last_counter;
    static uint16_t last_kecepatan;
    static uint16_t last_warna;
    static uint32_t lsRn;
    uint32_t Tmr = millis();

    if((Tmr-lsRn)>1000){
      lsRn = Tmr;
      counter++;
      if(counter>60){
        counter=0;
      }
    }
    if(last_kecepatan!=kecepatan || last_warna!=warna || last_counter!=counter){
      last_kecepatan=kecepatan;
      last_warna=warna;
      last_counter=counter;
      
      arr.clear();
      arr.set("/[0]", warna);
      arr.set("/[1]", counter);
      arr.set("/[2]", kecepatan);
      if (Firebase.set(fbdo2, "/monitor", arr))
      {
          Serial.println("PASSED");
          Serial.println("PATH: " + fbdo2.dataPath());
          Serial.println("TYPE: " + fbdo2.dataType());
          Serial.print("VALUE: ");
          Serial.println("------------------------------------");
          Serial.println();
      }
      else
      {
          Serial.println("FAILED");
          Serial.println("REASON: " + fbdo2.errorReason());
          Serial.println("------------------------------------");
          Serial.println();
      }
    }

    

}

void buatData(FirebaseData &fbdo){
  Serial.println("------------------------------------");
        Serial.println("Set Array Awal...");

        arr.clear();
        arr.set("/[0]", "resi0");
        arr.set("/[1]", "resi1");
        arr.set("/[4]", "resi4");
        if (Firebase.set(fbdo, path + "/Perintah", arr))
        {
            Serial.println("PASSED");
            Serial.println("PATH: " + fbdo.dataPath());
            Serial.println("TYPE: " + fbdo.dataType());
            Serial.print("VALUE: ");
            Serial.println("------------------------------------");
            Serial.println();
        }
        else
        {
            Serial.println("FAILED");
            Serial.println("REASON: " + fbdo.errorReason());
            Serial.println("------------------------------------");
            Serial.println();
        }
}

void printResult(FirebaseData &data, bool isi)
{

    if (data.dataType() == "int")
        Serial.println(data.intData());
    else if (data.dataType() == "float")
        Serial.println(data.floatData(), 5);
    else if (data.dataType() == "double")
        printf("%.9lf\n", data.doubleData());
    else if (data.dataType() == "boolean")
        Serial.println(data.boolData() == 1 ? "true" : "false");
    else if (data.dataType() == "string")
        Serial.println(data.stringData());
    else if (data.dataType() == "json")
    {
        Serial.println();
        FirebaseJson &json = data.jsonObject();
        //Print all object data
        Serial.println("Pretty printed JSON data:");
        String jsonStr;
        json.toString(jsonStr, true);
        Serial.println(jsonStr);
        Serial.println();
        Serial.println("Iterate JSON data:");
        Serial.println();
        size_t len = json.iteratorBegin();
        String key, value = "";
        int type = 0;
        for (size_t i = 0; i < len; i++)
        {
            json.iteratorGet(i, type, key, value);
            Serial.print(i);
            Serial.print(", ");
            Serial.print("Type: ");
            Serial.print(type == FirebaseJson::JSON_OBJECT ? "object" : "array");
            if (type == FirebaseJson::JSON_OBJECT)
            {
                Serial.print(", Key: ");
                Serial.print(key);
            }else{
              if(isi){
//                resi[i] = value;
//                Serial.print(", Resi: ");
//                Serial.println(resi[i]);
              }
            }
            
        }
        json.iteratorEnd();
    }
    else if (data.dataType() == "array")
    {
//        Serial.println();
        //get array data from FirebaseData using FirebaseJsonArray object
        FirebaseJsonArray &arr = data.jsonArray();
        //Print all array values
//        Serial.println("Pretty printed Array:");
        String arrStr;
        arr.toString(arrStr, true);
//        Serial.println(arrStr);
//        Serial.println();
//        Serial.println("Iterate array values:");
//        Serial.println();
        int n =arr.size();
        for (size_t i = 0; i < n; i++)
        {
//            Serial.print(i);
//            Serial.print(", Value: ");

            FirebaseJsonData &jsonData = data.jsonData();
            //Get the result data from FirebaseJsonArray object
            arr.get(jsonData, i);
            if (jsonData.typeNum == FirebaseJson::JSON_BOOL)
                Serial.println(jsonData.boolValue ? "true" : "false");
            else if (jsonData.typeNum == FirebaseJson::JSON_INT)
                Serial.println(jsonData.intValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_FLOAT)
                Serial.println(jsonData.floatValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_DOUBLE)
                printf("%.9lf\n", jsonData.doubleValue);
            else if (jsonData.typeNum == FirebaseJson::JSON_STRING ||
                     jsonData.typeNum == FirebaseJson::JSON_NULL ||
                     jsonData.typeNum == FirebaseJson::JSON_OBJECT ||
                     jsonData.typeNum == FirebaseJson::JSON_ARRAY){
                      resi[i] = jsonData.stringValue;
//                      Serial.println(jsonData.stringValue);
                     }
//                
        }
//        Serial.println();

        power = resi[0].toInt();
//        for(int i=0; i<n;i++){
//          Serial.printf("Data ke-%d ", i);
//          Serial.println(resi[i]);
//        }
    }
    else if (data.dataType() == "blob")
    {

        Serial.println();

        for (int i = 0; i < data.blobData().size(); i++)
        {
            if (i > 0 && i % 16 == 0)
                Serial.println();

            if (i < 16)
                Serial.print("0");

            Serial.print(data.blobData()[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
    }
    else if (data.dataType() == "file")
    {

        Serial.println();

        File file = data.fileStream();
        int i = 0;

        while (file.available())
        {
            if (i > 0 && i % 16 == 0)
                Serial.println();

            int v = file.read();

            if (v < 16)
                Serial.print("0");

            Serial.print(v, HEX);
            Serial.print(" ");
            i++;
        }
        Serial.println();
        file.close();
    }
    else
    {
        Serial.println(data.payload());
    }
}
