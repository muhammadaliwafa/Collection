int ty_puasa;
int hd_puasa;
//
//const char static namaPuasa[] PROGMEM    = {
//                                            "SYAWAL\0\0\0\0\0\0\0"
//                                            "SYA'BAN\0\0\0\0\0\0"
//                                            "SENIN\0\0\0\0\0\0\0\0"
//                                            "KAMIS\0\0\0\0\0\0\0\0"
//                                            "TENGAH BULAN\0"
//                                            "AROFAH\0\0\0\0\0\0\0"
//                                            "DZULHIJAH\0\0\0\0"
//                                            "TASU'A\0\0\0\0\0\0\0"
//                                            "ASYURA\0\0\0\0\0\0\0"
//                                            "RAMADHAN\0\0\0\0\0"
// 
//                                            };
char namaPuasa[][13]={
  "SYAWAL",
  "SYA'BAN",
  "SENIN",
  "KAMIS",
  "TENGAH BULAN",
  "AROFAH",
  "DZULHIJAH",
  "TASU'A",
  "ASYURA",
  "RAMADHAN"
};
                                            
void check_puasa()
  {
    // hitung tanggal besok Hjr_Month
//    hijir_date tmrH = toHijri(now.year(),now.month(),now.day(),1); 
    if(floatnow>stimeFloat[0] and floatnow<stimeFloat[6]){//puasa ditampilkan hanya pada malam hari
      ty_puasa = 0;
      Serial.println("return");
      return;
    }
//    Serial.println(Hjr_Hari);
    ty_puasa = 0;
    // cek hari dilarang puasa
    if (  ((Hjr_Month == 12)  and   (Hjr_Date == 10)) or   // Idul Adha
          ((Hjr_Month == 12)  and   (Hjr_Date == 11)) or   // Hari Tarsik 1
          ((Hjr_Month == 12)  and   (Hjr_Date == 12)) or   // Hari Tarsik 2
          ((Hjr_Month == 12)  and   (Hjr_Date == 13)) or   // Hari Tarsik 3
          ((Hjr_Month == 10)  and   (Hjr_Date ==  1)) or   // Idul Fitri
          ((Hjr_Month ==  8)  and   (Hjr_Date == 29)) or   // Akhir bulan Sya'ban
          ((Hjr_Month ==  8)  and   (Hjr_Date == 30)))
        {
          ty_puasa = 0;
        }
    else 
        {
          if ((Hjr_Month == 10) and (Hjr_Date > 2))  // Bulan Syawal --> Puasa 6 Hari Sawal
            {
              ty_puasa = 1;  
              hd_puasa = 2;
            }
          if ((Hjr_Month == 8) and (Hjr_Date >= 1))  // Perbanyak puasa sunnah bulan Sya'ban
            {
              ty_puasa = 2;
              hd_puasa = 3;
            }  
        if(Hjr_Hari== 2)  // Jika sekarang Minggu --> besok puasa Senin
            {
              ty_puasa = 3;
              hd_puasa = 1;
            }
        if(Hjr_Hari== 5)  // Jika sekarang Rabu --> besok puasa Kamis
            {
              ty_puasa = 4;
              hd_puasa = 1;
            }
          if(Hjr_Date == 13 or Hjr_Date == 14 or Hjr_Date == 15)   // Puasa Tengah Bulan tgl 13,14,15
            {
              ty_puasa = 5;
              hd_puasa = 1;
            }
          if ((Hjr_Month == 12) and (Hjr_Date == 9))  // 9 Dzulhijah --> Puasa Arofah
            {
              ty_puasa = 6;
              hd_puasa = 1;
            }
          if ((Hjr_Month == 12) and (Hjr_Date < 9))  // 9 hari pertama Dzulhijah --> Puasa Awal Dzulhijah
            {
              ty_puasa = 7;
              hd_puasa = 1;
            }
          if ((Hjr_Month == 1) and (Hjr_Date == 9))  // 9 Muharram --> Puasa Arofah
            {
              ty_puasa = 8;
              hd_puasa = 1;
            }
          if ((Hjr_Month == 1) and (Hjr_Date == 10))  // 10 Muharram --> Puasa Asura
            {
              ty_puasa = 9;
              hd_puasa = 1;
            }
          if (Hjr_Month == 9)  // Puasa Ramadhan
            {
              ty_puasa = 10;
              hd_puasa = 4;
            }
        }
  }

//void dispPuasa(uint8_t& next){
//  check_puasa();
//  if(ty_puasa==0){
//    next++;
//    return;
//    Serial.println("tidak dikerjakan");
//  }
//
//  static uint32_t pM;
//  uint32_t cM = millis();
//  static uint8_t Kecepatan = 45;
//  static uint32_t x;
//
//  char msg[36];
//  char buff[6];
//  if (kdp) {
//    sprintf(buff, "%02d:%02d", rJam, rMen);
//  }
//  else {
//    sprintf(buff, "%02d|%02d", rJam, rMen);
//  }
//
//  
//
//
//  Disp.drawRect(0,3,31,3);
//  Disp.drawRect(0,15,31,15);
//  Disp.setFont(Font4x6ali);
//  Disp.drawText(4, -1, buff);
//  if(Hjr_Month == 9){
//    sprintf(msg,"JANGAN LUPA NIAT PUASA %s",namaPuasa[ty_puasa-1]);
//  }else
//  sprintf(msg,"WAKTU PUASA %s ESOK HARI",namaPuasa[ty_puasa-1]);
//
//  
////  int width = Disp.width();
////  Disp.setFont(kecil3x6);
////  int fullScroll = Disp.textWidth(msg) + width;
//
//  
//
//  if(cM - pM > Kecepatan) {
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
//      tmpjws = 0;
//      next +=1;
//      
//    }
//
////    Disp.drawText(width - x, 8, msg);
//    
//  }
//}
