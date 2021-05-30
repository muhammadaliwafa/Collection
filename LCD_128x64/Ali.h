
bool kdp;
int8_t SholatNow = -1;
bool azzan = false;
bool jumat = false;
//int width = Disp.width();
uint8_t dispj;

char namaWaktuSholat[][8] = {"IMSYAK", "SUBUH", "TERBIT", "DHUHA", "DZUHUR", "ASHAR", "MAGHRIB", "ISYA"};
char namaSholat[][8] = {"IMS", "SBH", "TBT", "DHA", "DHR", "ASR", "MGB", "ISY"};

void serialWithDelay(String msg, uint16_t dely);

//void tombol();


//void runAdzanSingle(uint8_t& next, bool rst){
//  static uint8_t    det;
//  static uint8_t    x;
//  static uint8_t    s; // 0=in, 1=out 
//  if(rst){
//    digitalWrite(2, LOW);
//    det = 0;
//    x=0;
//    s=0;
//    return;
//  }
//  
//             
//  static uint32_t   lsRn;
//  uint32_t          Tmr = millis();
//  char              msg[6]; 
//  static bool       scroll;
//  static uint8_t    durasi=10;
//  static uint8_t    lsDtk;
//
//  char Msg[8];
//  digitalWrite(2, kdp);
//  Disp.setFont(Font6x7);
//  if(jumat){
//    sprintf(Msg, "%s", "JUM'AT");
//  }else{
//    sprintf(Msg, "%s", namaWaktuSholat[SholatNow]);
//  }
//  
//  int selisih = int(Disp.width() - Disp.textWidth(Msg));
//  if(selisih<0){
//    scroll = true;
//    selisih -= 2;
//  }
//  else {
//    scroll = false;
//    
//  }
//  if(scroll){//penanda
//    if((Tmr-lsRn)>100) 
//    { 
//      if(s==0 and x<(-selisih)){lsRn=Tmr;x++;Disp.clear();}
//      if(s==1 and x>0){lsRn=Tmr;x--;Disp.clear();}
//    }
//    if((Tmr-lsRn)>500 and x==(-selisih)) {s=1;}
//    if((Tmr-lsRn)>500 and x==0 and s==1) {s=0;}
//    Disp.drawText(1-x, 9, Msg);
//  }
//  else{
//    uint8_t center = selisih/2;
//    Disp.drawText(center, 9, Msg);
//  }
//  
//  
//  Disp.setFont(Font4x6b);
//  if(kdp){
//    if(SholatNow == 0){
//      sprintf(msg, "%02s", "Waktu");//jika yang ditampilkan adalah imsyak
//    }else
//      sprintf(msg, "%02s", "ADZAN");
//  }
//  else sprintf(msg, "%02s", "     ");
//  uint8_t center = int((Disp.width() - Disp.textWidth(msg))/2);
//  Disp.drawText(center, 0, msg);
//  Disp.drawRect(3,7,28,7); // garis doang
//  if(lsDtk!=rDet){
//    lsDtk=rDet;
//    det++;
//
//  }
//  if(det>durasi){
//    Disp.clear();
//    digitalWrite(2, LOW);
////    azzan = false;
//    det = 0;
//    x=0;
//    s=0;
//    next--;
//  }
//  
//  
//}

//void drawMasjid(int x, int y){ //dibuat di dotmatrixtool.com 16x8 row major, big endian
//  static const uint8_t masjid[] PROGMEM =
//  {
//     15,8,
//     0x01, 0x02, 0x43, 0x8a, 0xe7, 0xc4, 0xef, 0xee, 0xaf, 0xea, 0xe7, 0xce, 0xbf, 0xfa, 0xff, 0xfe
//  };
//  Disp.drawBitmap(x, y, masjid);
//}
//
//void jamNormal(uint8_t x, uint8_t y) {
//  char buff[8];
//  if (kdp) {
//    sprintf(buff, "%02d;%02d;%02d", rJam, rMen, rDet);
//  }
//  else {
//    sprintf(buff, "%02d|%02d|%02d", rJam, rMen, rDet);
//  }
//
//  
//  Disp.drawText(x, y, buff);
//
//}
//
//void anim_DT(uint8_t& next){
//    
//    static uint8_t    y;
//    static uint8_t    s; // 0=in, 1=out              
//    static uint32_t   lsRn;
//    uint32_t          Tmr = millis();
//
////    char buff[8];
//    char buffdet[3];
//    
//
//    if((Tmr-lsRn)>100) 
//      { 
//        if(s==0 and y<8){lsRn=Tmr;y++;Disp.clear();}
//        if(s==1 and y>0 and !jumat){lsRn=Tmr;y--;Disp.clear();}//waktu khotbah akan menunjukan waktu saja
//      }
//    if((Tmr-lsRn)>10000 and y ==8) {
//        lsRn = Tmr;
//        if(!jumat){//waktu khotbah akan menunjukan waktu saja
//          s=1;
//        }
//      }
//    if (y==8)
//      {
////        Disp.drawRect(1,2,62,13);
//      }
//    
//
//    sprintf(buffdet, "%02d", rDet);
//  
//    Disp.setFont(Font6x7);
//    jamNormal(1,17-y);
//
//    
//    Disp.drawCircle(27,y-8,7,1);
//    Disp.setFont(Font3x5);
//    Disp.drawText(24, y-11, buffdet);
//    drawMasjid(3,y-8);
//    
//    if (y == 0 and s==1) {next +=1; s=0; y=0;}
//}



//void identitas(uint8_t& next) {
//  static uint32_t lsRn;
//  uint32_t tmr = millis();
//  static uint8_t Kecepatan = 20;
//  static uint32_t x;
//  Disp.drawRect(-1, 2, 65, 13);
//  char msg[54];
//  sprintf(msg, "JAM WAKTU SHOLAT MUSHOLLA AL-FURQON");
//  
//
//  int width = Disp.width();
//  Disp.setFont(Font6x7);
//  int fullScroll = Disp.textWidth(msg) + width;
//
//
//  if (tmr - lsRn > Kecepatan) {
//
//    lsRn = tmr;
//
//    if (x < fullScroll) {
//
//      ++x;
//
//    } else {
//      x = 0;
////      tmpjws = 0;
//      next += 1;
//      Disp.clear();
//
//    }
//    
//    Disp.drawText(width - x, 4, msg);
//
//  }
//
//}
//
//
//void dispTanggal(uint8_t type, uint8_t& next) { //0 masehi 1 hijriah
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//  static uint8_t Kecepatan = 35;
//  static uint32_t x;
//
//  char msg[20];
//  if (type == 0) {
//    sprintf(msg, "%s, %d %s %i", namaHari[rHar], rTgl, namaBulan[rBul], rTah);
//  }
//  else if (type == 1) {
//    sprintf(msg, "%d %s %i", Hjr_Date, bulanHijriah[Hjr_Month], Hjr_Year);
//  }
//
//  //  int width = Disp.width();
//  Disp.setFont(kecil3x6);
//  int fullScroll = Disp.textWidth(msg) + width;
//
//
//
//  if (cM - pM > Kecepatan) {
//
//    pM = cM;
//
//    if (x < fullScroll) {
//
//      ++x;
//
//    } else {
//
//      x = 0;
//      Disp.clear();
////      tmpjws = 0;
//      next += 1;
//
//    }
//
//    Disp.drawText(width - x, 9, msg);
//
//  }
//
//}
//
//
//void dispInfo(uint8_t& next) { //0 masehi 1 hijriah
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//  static uint8_t Kecepatan = 35;
//  static uint32_t x;
//  static bool type;
//
//  char msg[60];
//  if (type == 0) {
//    sprintf(msg, "%s", configjws.namamasjid);
//  }
//  else if (type == 1) {
//    sprintf(msg, "%s", configjws.informasi);
//  }
//
//  //  int width = Disp.width();
//  Disp.setFont(kecil3x6);
//  int fullScroll = Disp.textWidth(msg) + width;
//
//
//
//  if (cM - pM > Kecepatan) {
//
//    pM = cM;
//
//    if (x < fullScroll) {
//
//      ++x;
//
//    } else {
//
//      x = 0;
//      Disp.clear();
//      type= !type;
////      tmpjws = 0;
//      next += 1;
//
//    }
//
//    Disp.drawText(width - x, 9, msg);
//
//  }
//
//}
//
//
//
//
//
//void runAzzan(uint8_t& next, bool rst) {
//  static uint32_t lsRn;
//  static uint32_t lsRnd;
//  static uint16_t det;
//  uint32_t tmr = millis();
//  BunyiBeep(2, 8);
//  if(rst){
//    det = 0;
//  }
//
//
//  if (tmr - lsRn >= 1000) {
//    det++;
//    lsRn = tmr;
//    lsRnd = tmr;
//  }
//  if (det < 10) {
//    if (kdp) {
//      Disp.drawRect(1, 2, 62, 13);
//      Disp.setFont(Font6x7);
//      Disp.drawText(14, 1, "ADZAN");
//      char Msg[8];
//      sprintf(Msg, "%s", namaWaktuSholat[SholatNow]);
//      int center = int((Disp.width() - Disp.textWidth(Msg)) / 2);
//      Disp.drawText(center, 9, Msg);
//      digitalWrite(2, kdp);
//    } else
//      Disp.clear();
//      digitalWrite(2, kdp);
//  }
//  else if (det == 10){
//    Disp.clear();
//    det++;
//    digitalWrite(2, LOW);
//  }
//  else if (det > 10) {
////    Disp.drawRect(1, 2, 62, 13);
//    digitalWrite(2, LOW);
//    Disp.setFont(Font6x7);
//    jamNormal(0,1);
//    Disp.setFont(kecil3x6);
//    char Msg[15];
//    if (!kdp) {
//      sprintf(Msg, "ADZAN %s", namaWaktuSholat[SholatNow]);
//    } else {
//      sprintf(Msg, "               ");
//    }
//
//    int center = int((Disp.width() - Disp.textWidth(Msg)) / 2);
//    Disp.drawText(center, 9, Msg);
//    //      String sholat = namaWaktuSholat[SholatNow];
//    //      digitalWrite(2, HIGH);
//    //      Serial.println("tampil azzan "+ sholat+" Selesai");
//
//  }
//  if(floatnow>(stimeFloat[SholatNow] + (float)configjws.durasiadzan/60)){
//    Disp.clear();
//    azzan = false;
//    det = 0;
//    next--;
//  }
//
//}
void disableAzzan(){
  if(azzan){
    float durasiazz = configjws.durasiadzan;
    durasiazz = durasiazz/60;
      if(SholatNow==0){
          if(floatnow>(stimeFloat[SholatNow]+0.03)){
          azzan = false;
          Serial.println("adzan false");
        }
      }
    if(floatnow>(stimeFloat[SholatNow]+durasiazz)){
      azzan = false;
      Serial.println("adzan false");
    }
  }
  if(jumat){
    float durasiktbh = configjws.durasikutbah;
    durasiktbh=durasiktbh/60;
//    Serial.println(durasiktbh);
    if(floatnow>(stimeFloat[SholatNow]+durasiktbh)){
      jumat = false;
    }
  }
  
}

void check_azzan()
{ 
  disableAzzan();
  for (int i = 0; i <= 7; i++)
  {
    if(!configjws.ringimsyak and i==0){//optional skip imsyak
      i++;
    }
    if (i != 2 and i != 3) // bukan terbit dan bukan dhuha
    {
      if (floatnow >= stimeFloat[i])
      {
        if (!azzan and (floatnow > stimeFloat[i]) and (floatnow < (stimeFloat[i] + 0.03)))
        {
          //                    if(daynow ==6 and SholatNow ==4 and Prm.MT==1) {jumat=true;}
//          Disp.clear();
          SholatNow = i;
          if(i==6){
            TanggalHijriah();
          }
          if(rHar ==5 and SholatNow==4){jumat = true;}
          Serial.print(rHar);
          
          azzan = true;
          tmputama = 1;
//          runAzzan(tmputama, true);
          String sholat = namaWaktuSholat[i];
          digitalWrite(2, LOW);
          Serial.println("waktu sholat " + sholat);
//          Serial.println(rHar);
        }

      }
    }
  }
}

void kedip() {
  static uint32_t   timerSave, lastSecond;
  uint32_t          nowmilis = millis();
  if (lastSecond != rDet) {
    lastSecond = rDet;
    kdp = true;
    timerSave = nowmilis;
  }
  if ((nowmilis - timerSave) > 500) {
    kdp = false;
  }
}




//void dispJadwal(uint8_t& next) {//untuk 2 panel
//
//  static uint8_t    y;
//  static uint8_t    s; // 0=in, 1=out
//  static uint32_t   lsRn;
//  static uint8_t    sNum;
//  uint32_t          Tmr = millis();
//  if (sNum == 0 and !configjws.dispimsyak) sNum = 1;
//  if (sNum == 2) sNum = 3;
//  if (sNum == 3 and !configjws.dispdhuha) sNum = 4;
//
//  if ((Tmr - lsRn) > 30)
//  {
//    if (s == 0 and y < 8) {
//      lsRn = Tmr;
//      y++;
//    }
//    if (s == 1 and y > 0) {
//      lsRn = Tmr;
//      y--;
//    }
//  }
//  if ((Tmr - lsRn) > 2000 and y == 8) {
//    s = 1;
//    //    Serial.println(stimeInt[1][0]);
//
//  }
//  if (y == 0 and s == 1) {
//    s = 0;
//    if (sNum < 7) {
//      sNum++;
//    } else {
//      sNum = 0;
//      next = next + 1;
//    }
//
//
//  }
//  Disp.setFont(ElektronMart5x6);
//  char buffJ[10];
//  sprintf(buffJ, "%02s %02i:%02i", namaWaktuSholat[sNum], stimeInt[sNum][0], stimeInt[sNum][1]);
//  int   tw = Disp.textWidth((String)buffJ);
//  int   c = int((64 - tw) / 2);
//  Disp.drawText(c, 16 - y, buffJ);
//
//}
//
//
//void dispJadwal2(uint8_t& next) {// untuk 1 panel
//
//  static int    y = 0;
//  static uint8_t    s; // 0=in, 1=out
//  static uint32_t   lsRn;
//  static uint8_t    sNum;
//  uint32_t          Tmr = millis();
//  if (sNum == 0 and !configjws.dispimsyak) sNum = 1;
//  if (sNum == 2) sNum = 3;
//  if (sNum == 3 and !configjws.dispdhuha) sNum = 4;
//
//  if ((Tmr - lsRn) > 20)
//  {
//    if (s == 0 and y < 26) {
//      lsRn = Tmr;
//      y++;
//      Disp.clear();
//    }
//    if (s == 1 and y > 0) {
//      lsRn = Tmr;
//      y--;
//      Disp.clear();
//    }
//  }
//  if ((Tmr - lsRn) > 2000 and y == 26) {
//    s = 1;
//    //    Serial.println(stimeInt[1][0]);
//
//  }
//  
//
//  
//
//  char buffJ[6];
//  char buffN[4];
//  char buff[8];
//  sprintf(buffN, "%02s", namaSholat[sNum]);
//  sprintf(buffJ, "%02i:%02i", stimeInt[sNum][0], stimeInt[sNum][1]);
//
//  if (kdp) {
//    sprintf(buff, "%02d:%02d", rJam, rMen);
//  }
//  else {
//    sprintf(buff, "%02d|%02d", rJam, rMen);
//  }
//  
//  int   tw = Disp.textWidth((String)buffJ);
//  int   c = int((width - tw) / 2);
//  
//
//  
////  Disp.drawFilledRect(0,8,31,15,0);//background text untuk remove buffer
////  Disp.drawFilledRect(0,2,14,7,0);
//  
//
//  Disp.setFont(Font3x5);
//  Disp.drawText(1, 0, buffN);
//  Disp.drawText(15, -2, buff);
//  Disp.setFont(Font4x6ali);
//  Disp.drawText(1, 8, buffJ);
//  
//  Disp.drawFilledRect(0,1,13,1,1); // garis doang
//  Disp.drawFilledRect(15,7,31,7,1);
//
//
//  Disp.drawFilledRect(13-y,2,13-y,7,1); // anim atas
//  Disp.drawFilledRect(-1,2,12-y,7,0);
//
//  Disp.drawFilledRect(y,8,y,15,1); //anim bawah
//  Disp.drawFilledRect(y+1,8,25,15,0);
////  Disp.drawFilledRect(y,8,y,15,1);
//  if (y == 0 and s == 1) {
//      s = 0;
//      if (sNum < 7) {
//        sNum++;
//      } else {
//        sNum = 0;
//        next = next + 1;
//        Disp.clear();
//      }
//  
//  
//    }
//
//
//}




void waktuSholatNow() {
  for (int i = 7; i >= 0; i--) {
    Serial.println(waktuSholat(i));
    if (floatnow > waktuSholat(i)) {
      SholatNow = i;
      break;
    }
  }
  Serial.printf("waktu sekarang adalah %d", SholatNow);
  Serial.println();

}
