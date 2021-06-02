#include <TimerOne.h>
#include <SPI.h>
#include <JadwalSholat.h>

#include <EEPROM.h>
#include <avr/pgmspace.h>

#include <Wire.h>
#include <RtcDS3231.h>
RtcDS3231<TwoWire> Rtc(Wire);

#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

SoftwareSerial mySoftwareSerial(3,2); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
#define buzz 8
#include "RTC.h"
#include "ParamEEPROM.h"
#include "7SegmentSPI.h"


int adzan = false, tarhim = false;
bool kdp, blinking;
int8_t SholatNow = -1;

float stimeFloat[]={0,0,0,0,0,0,0,0};
uint8_t stimeInt[8][3]={
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0},
  {0,0,0}
};

void setup() {
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  GetPrm();
  
  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
//    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  myDFPlayer.volume(vol);  //Set volume value (0~30).
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_U_DISK);
  myDFPlayer.playMp3Folder(3);
  
  
  pinMode(SS_PIN, OUTPUT);  
  SPI.setBitOrder(MSBFIRST);   // Reverse the SPI Data o/p. 
  SPI.begin();                 // Start SPI

  mulaiRTC();
  UpdateWaktu();
  waktuSholatNow();
  
  pinMode(OUTPUT_ENABLE_PIN, OUTPUT);
  digitalWrite(OUTPUT_ENABLE_PIN, 0);
  
  pinMode(11, OUTPUT);
  pinMode(buzz, OUTPUT);
//  digitalWrite(buzz, 0);
 tampil();
 pinMode(4, OUTPUT);
 pinMode(5, INPUT);
 digitalWrite(4, HIGH);
}

void loop() {

  UpdateWaktu();
  cekAdzan();
  runAdzan();
  RTC2digit();
  kedip();
  
}

void runAdzan(){
  static uint8_t runblink;
  static bool lstKdp;
  if(!adzan){
    runblink =0;
    return;
  }
  if(lstKdp!=kdp){
    lstKdp=kdp;
    
    if(runblink<=12){
      runblink++;
    }else blinking=false;
  }
//  Serial.println(digitalRead(5));
  if(digitalRead(5) && !blinking){
    adzan = false;
    tarhim = false;
    Serial.println("Adzan False");
  }
  
} 
void cekAdzan(){
  if(SholatNow+1==0 && SholatNow+1==1){
    return;
  }
  float selisih = floatnow - stimeFloat[SholatNow+1];
//  Serial.print(stimeFloat[SholatNow+1], 5);
//  Serial.print("==");
//  Serial.println(selisih, 5);
  if(!adzan){
    if(selisih>0 && selisih<0.03){
//    if(selisih>0 && selisih<0.007){
        adzan = true;
        blinking=true;
        Serial.println("Adzan True");
        myDFPlayer.volume(vol);
      if(SholatNow==1){
        myDFPlayer.playMp3Folder(6);
      }
      else {
//        myDFPlayer.playMp3Folder(7);
        myDFPlayer.playMp3Folder(1);
      }
      waktuSholatNow();
    }
  }
  if(!tarhim && selisih<0 && selisih>-0.081666667){
//    if(!tarhim && selisih<0 && selisih>-0.00085){
//    play MP3 0002.mp3
    myDFPlayer.volume(vol);
    myDFPlayer.playMp3Folder(2);
//    myDFPlayer.playMp3Folder(8);
    tarhim = true;
  }
}



void updateJWS(){
  float koreksi = 2;
  koreksi = (float)koreksi/60;
  sholatCal(rTah, rBul, rTgl, Prm.L_TZ, Prm.L_LA, Prm.L_LO, Prm.L_AL);
  for(int i=0; i<=7; i++){
  float   stime   = waktuSholat(i)+koreksi;
  stimeFloat[i] = stime;
  stimeInt[i][0] = floor(stime);
  stimeInt[i][1] = floor((stime-(float)stimeInt[i][0])*60);
  stimeInt[i][2] = floor((stime-(float)stimeInt[i][0]-(float)stimeInt[i][1]/60)*3600);
  }
  for(int i=0; i<=7; i++){
    char  out[21];
    sprintf(out, "sholat %01d %02d : %02d : %02d", i, stimeInt[i][0], stimeInt[i][1], stimeInt[i][2]);
    Serial.println(out);
    
  }
}

void UpdateWaktu() {

  static long pM;
  static uint16_t d;
  static uint8_t lastTgl;
  static uint8_t lastMnt;
  
  BacaRTC();
  if(lastTgl!=rTgl){
    updateJWS();
    lastTgl=rTgl;
  }
  
}

void waktuSholatNow() {
  SholatNow=-1;
  for (int i = 7; i >= 0; i--) {
    Serial.print(stimeFloat[i], 5);
    Serial.print("==");
    Serial.println(floatnow);
    if (floatnow > stimeFloat[i]) {
      Serial.println("Sholat Now");
      SholatNow = i;
      break;
    }
  }

  Serial.println(SholatNow);

}

void kedip() {
  static uint32_t   timerSave, lastSecond;
  uint32_t          nowmilis = millis();
  if (lastSecond != rDet) {
    lastSecond = rDet;
    kdp = true;
    digitalWrite(4, kdp);
    timerSave = nowmilis;
  }
  if ((nowmilis - timerSave) > 500) {
    kdp = false;
    digitalWrite(4, kdp);
    tampilJumat();
  }
//  digitalWrite(buzz, kdp);
  if(blinking){
    digitalWrite(OUTPUT_ENABLE_PIN, !kdp);
    digitalWrite(buzz, kdp);
  }else{
    digitalWrite(OUTPUT_ENABLE_PIN, LOW);
    digitalWrite(buzz, LOW);
  }
}

void tampilJumat(){
  if((rHar==4 && SholatNow>=6) || (rHar==5 && SholatNow<4)){
    analogWrite(6, 10);
  }
  else digitalWrite(6, LOW);
}
