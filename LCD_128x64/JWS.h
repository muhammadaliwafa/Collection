
void textCenter(int y,String Msg) {
  
  int center = int((Disp.width()-Disp.textWidth(Msg)) / 2);
  Disp.drawText(center,y,Msg);
  
}
//----------------------------------------------------------------------
// HITUNG MUNDUR WAKTU SETELAH ADZAN SAMPAI MULAI IQOMAH

void Iqomah(bool rst) {
  if(!configjws.dispiqomah){
    tmputama = 0;
    return;
  }
  if(!rst){
    detikbeep = 0;
    detikiqmh = 60;
    return;
  }
  BunyiBeep(1,2);
  static uint32_t pM;
  uint32_t cM = millis();
  static char hitungmundur[6];  
//  tombol();

  Disp.setFont(ElektronMart5x6);
  textCenter(0, "IQOMAH");

//  if (detikiqmh == 60) {
//    detikiqmh = 59;
//  }
  sprintf(hitungmundur, "%02d:%02d", menitiqomah, detikiqmh);
  Disp.setFont(ElektronMart6x8);
  textCenter(8, hitungmundur);
  if (cM - pM >= 1000) {
    pM = cM;
    detikiqmh--;

    if (menitiqomah == 0 && detikiqmh == 0) {
      Disp.clear();
      detikbeep = 0;
      detikiqmh = 59;
      tmputama = 3;
//      in = false;
//      azzan = false;
    }

    if (detikiqmh < 0) {
      detikiqmh = 59;
      menitiqomah--;
      
//      Serial.println(menitiqomah); ini jalan
    }
    
  }

  
  
}



//----------------------------------------------------------------------
// TAMPILAN SAAT SHOLAT

void TampilSaatSholat() {

  BunyiBeep(1,4);

  static uint32_t pM;
  uint32_t cM = millis();
  static uint32_t durasi = 20; // Detik
  static uint32_t det;

  Disp.setFont(ElektronMart5x6);
  if(kdp){
    textCenter(0, "RAPATKAN");
  }else
  textCenter(0, "LURUSKAN");
  
  textCenter(8, "SHAF");

  if (cM - pM >= 1000) {
    pM = cM;
    det++;

    if (durasi == det) {
      azzan=false;
      Disp.clear();
      detikbeep = 0;
      det = 0;
      tmputama = 0;
    }
  }  
  
}
