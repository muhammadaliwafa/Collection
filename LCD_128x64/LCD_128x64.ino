/*

  GraphicsTest.ino

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#include <JadwalSholat.h>

#include <FS.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>

#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);
#include <F1kM_Hisab.h>

#include <TimeLib.h>

#include "Buzzer.h"
#include "RTC.h"
#include "CalculateTime.h"
#include "FSConfig.h"
#include "WebServer.h"
#include "WebPage.h"
#include "Ali.h"
//#include "JWS.h"
#include "Puasa.h"
#include "Button.h"


bool enableSettingMode = true;
/*
  U8g2lib Example Overview:
    Frame Buffer Examples: clearBuffer/sendBuffer. Fast, but may not work with all Arduino boards because of RAM consumption
    Page Buffer Examples: firstPage/nextPage. Less RAM usage, should work with all Arduino boards.
    U8x8 Text Only Example: No RAM usage, direct communication with display controller. No graphics, 8x8 Text only.
    
*/

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ D5, /* data=*/ D7, /* CS=*/ D8, /* reset=*/ D0);



void u8g2_prepare(void) {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}



uint8_t draw_state = 0;

void draw(void) {
//  Serial.print(draw_state);Serial.print("--");Serial.println(draw_state>>3);
  u8g2_prepare();
  char buff[10];
  char buffJ[30];
  uint8_t yC=8;
  sprintf(buffJ, "Shubuh  : %02i:%02i", stimeInt[1][0], stimeInt[1][1]);
  u8g2.drawStr( 0, 9+yC, buffJ);
  sprintf(buffJ, "Dzuhur  : %02i:%02i", stimeInt[4][0], stimeInt[4][1]);
  u8g2.drawStr( 0, 18+yC, buffJ);
  sprintf(buffJ, "Ashar   : %02i:%02i", stimeInt[5][0], stimeInt[5][1]);
  u8g2.drawStr( 0, 27+yC, buffJ);
  sprintf(buffJ, "Maghrib : %02i:%02i", stimeInt[6][0], stimeInt[6][1]);
  u8g2.drawStr( 0, 36+yC, buffJ);
  sprintf(buffJ, "Isya    : %02i:%02i", stimeInt[7][0], stimeInt[7][1]);
  u8g2.drawStr( 0, 45+yC, buffJ);
  sprintf(buff, "%02d:%02d:%02d", rJam, rMen, rDet);  
  u8g2.drawStr( 32, 0, buff);

}


void setup(void) {
  u8g2.begin();
  Serial.begin(9600);
  mulaiRTC();

  LittleFS.begin();
  loadJwsConfig(fileconfigjws, configjws);

  //WIFI
  //  int sensorValue = analogRead(A0);
  //  if(sensorValue<200){
  //    enableSettingMode = true;
  //    Serial.println("enable Setting");
  //  }else Serial.println("setting disable");
  //  pinMode(pin_led, OUTPUT);
  if (enableSettingMode) {
    Serial.println("WIFI STARTED");
    wifiConnect();

    server.on("/", []() {
      server.send_P(200, "text/html", setwaktu);

      // Kalau ada perubahan tanggal
      if (server.hasArg("date")) {

        uint16_t jam;
        uint8_t menit;
        uint8_t detik;
        String sd = server.arg("date");
        String lastSd;

        jam = ((sd[0] - '0') * 1000) + ((sd[1] - '0') * 100) + ((sd[2] - '0') * 10) + (sd[3] - '0');
        menit = ((sd[5] - '0') * 10) + (sd[6] - '0');
        detik = ((sd[8] - '0') * 10) + (sd[9] - '0');

        if (sd != lastSd) {
          RtcDateTime now = Rtc.GetDateTime();
          uint8_t hour = now.Hour();
          uint8_t minute = now.Minute();
          Rtc.SetDateTime(RtcDateTime(jam, menit, detik, hour, minute, 0));
          lastSd = sd;
        }

        BacaRTC();

        server.send ( 404 , "text", message );

      }

      // Kalau ada perubahaan jam
      if (server.hasArg("time")) {

        String st = server.arg("time");
        String lastSt;
        uint8_t jam = ((st[0] - '0') * 10) + (st[1] - '0');
        uint8_t menit = ((st[3] - '0') * 10) + (st[4] - '0');

        if (st != lastSt) {
          RtcDateTime now = Rtc.GetDateTime();
          uint16_t year = now.Year();
          uint8_t month = now.Month();
          uint8_t day = now.Day();
          Rtc.SetDateTime(RtcDateTime(year, month, day, jam, menit, 0));
          lastSt = st;
        }

        BacaRTC();
        updateJWS();
        TanggalHijriah();
        //         bacaParameter();

        server.send ( 404 , "text", message );


      }
    });

    server.on("/selaraskan", HTTP_POST, handleSelaraskan);
//    server.on("/kecerahan", HTTP_POST, handleKecerahan);
    server.on("/simpandatajws", HTTP_POST, handleSettingJwsUpdate);

    server.on("/xmlwaktu", handleXMLWaktu); // http://192.168.4.1/xmlwaktu
    server.on("/xmldatajws", handleXMLDataJWS); // http://192.168.4.1/xmldatajws


    httpUpdater.setup(&server);
    server.begin();
    Serial.println("HTTP server started");

  }
//    membuatDataAwal();
  // Tampilkan Parameter yang tersimpan
  bacaParameter();


  BacaRTC();
  updateJWS();
  TanggalHijriah();
  waktuSholatNow();
  char hij[25];
  sprintf(hij, "tanggal %d %s", Hjr_Date, bulanHijriah[Hjr_Month]);
  Serial.println(hij);


  if (!configjws.dispiden) { //jangan tampilkan text pembuka
    dispj = 1;
  }
}

void loop(void) {
  if (enableSettingMode) {
    server.handleClient();
  }
  // picture loop
  BacaRTC();  
  UpdateWaktu();
  u8g2.clearBuffer();
  draw();
  u8g2.sendBuffer();
  
  
  // increase the state
//  draw_state++;
//  if ( draw_state >= 12*8 )
//    draw_state = 0;

  // deley between each page
//  delay(100);

}

void drw(){
  static uint32_t lsRn;
  uint32_t tmr=millis();

  if((tmr-lsRn)>200){
    lsRn=tmr;
    BacaRTC();  
    u8g2.clearBuffer();
    draw();
    u8g2.sendBuffer();
  }
  
}
