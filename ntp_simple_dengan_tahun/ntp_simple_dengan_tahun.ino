#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid     = "all";
const char *password = "qwerty234";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

//Week Days
String weekDays[7]={"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

//Month names
String months[12]={"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

void setup() {
  // Initialize Serial Monitor
  Serial.begin(9600);
  
  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(28800);
}

void loop() {
  Serial.println("CONNECTED");
  timeClient.update();
  String waktu = "SDT";

  unsigned long epochTime = timeClient.getEpochTime();
//  Serial.print("Epoch Time: ");
//  Serial.println(epochTime);
  
  String formattedTime = timeClient.getFormattedTime();
//  Serial.print("Formatted Time: ");
//  Serial.println(formattedTime);  

  int currentHour = timeClient.getHours();
//  Serial.print("Hour: ");
//  Serial.println(currentHour);  

  int currentMinute = timeClient.getMinutes();
//  Serial.print("Minutes: ");
//  Serial.println(currentMinute); 
   
  int currentSecond = timeClient.getSeconds();
//  Serial.print("Seconds: ");
//  Serial.println(currentSecond);  

  String weekDay = weekDays[timeClient.getDay()];
//  Serial.print("Week Day: ");
//  Serial.println(weekDay);    

  //Get a time structure
  struct tm *ptm = gmtime ((time_t *)&epochTime); 

  int monthDay = ptm->tm_mday;
//  Serial.print("Month day: ");
//  Serial.println(monthDay);
  if (monthDay<10){
    waktu += "0" + String(monthDay);
  }else{
    waktu += String(monthDay);
  }

  
  int currentMonth = ptm->tm_mon+1;
//  Serial.print("Month: ");
//  Serial.println(currentMonth);
  if (currentMonth<10){
    waktu += "0" + String(currentMonth);
  }else{
    waktu += String(currentMonth);
  }

  

  String currentMonthName = months[currentMonth-1];
//  Serial.print("Month name: ");
//  Serial.println(currentMonthName);

  int currentYear = ptm->tm_year+1900;
//  Serial.print("Year: ");
//  Serial.println(currentYear);
  waktu += String(currentYear - 2000);

  if (currentHour<10){
    waktu += "0" + String(currentHour);
  }else{
    waktu += String(currentHour);
  }


  if (currentMinute<10){
    waktu += "0" + String(currentMinute);
  }else{
    waktu += String(currentMinute);
  }

  if (currentSecond<10){
    waktu += "0" + String(currentSecond);
  }else{
    waktu += String(currentSecond);
  }

  waktu += "3";
  
  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);
//  Serial.print("Current date: ");
//  Serial.println(currentDate);
//
//  Serial.println("yang akan dikirim :");
  

  Serial.println(waktu);
  delay(500);
  Serial.println("+DISC:SUCCESS");

  delay(3000);
}
