// ----------------------
// FS CONFIG

int detikiqmh;
int menitiqomah;
uint8_t tipealarm;
uint8_t tmputama;
uint8_t tmpjws;
uint8_t tmpinfo;
uint8_t Hjr_Hari;

struct ConfigJws {
  bool dispiqomah;
  uint8_t iqmhs; // menit
  uint8_t iqmhd; // menit
  uint8_t iqmha; // menit
  uint8_t iqmhm; // menit
  uint8_t iqmhi; // menit
  uint8_t durasiadzan; // Menit
  uint8_t ihti; // Koreksi Waktu Menit Jadwal Sholat
  float latitude;
  float longitude;
  int8_t zonawaktu;
  uint8_t hilal;
//  uint8_t cerah;
  int8_t koreksihjr;
  bool dispimsyak;
  bool ringimsyak;
  bool disppuasa;
  bool dispiden;
  bool dispdhuha;
  char namamasjid[512];
  char informasi[512];
  char ssid[30];
  char password[30];
  uint8_t durasikutbah;
};

struct ConfigDisp {
  int cerah;
};

const char *fileconfigjws = "/configjws.json";
ConfigJws configjws;

const char *fileconfigdisp = "/configdisp.json";
ConfigDisp configdisp;

String message, XML;


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

float stimeFloat[]={0,0,0,0,0,0,0,0};

char dispNJadwal[] ="";

void updateJWS(){
//  int Tahun = 2020, 
//      Bulan = 12,
//      Tgl =  30;
//  float ZonaWaktu = 7, 
//        Latitude = -6.2311, //jakarta
//        Longitude = 106.7791, 
//        Altitude = 45;
//  
//  sholatCal(Tahun, Bulan, Tgl, ZonaWaktu, Latitude, Longitude, Altitude); 
  double koreksi = configjws.ihti;
  koreksi = (double)koreksi/60;//-8.11200061906755,115.088181495667
//  sholatCal(rTah, rBul, rTgl, configjws.zonawaktu, -8.11200061906755, 115.088181495667, 45);
  sholatCal(rTah, rBul, rTgl, configjws.zonawaktu, configjws.latitude, configjws.longitude, 45);
  for(int i=0; i<=7; i++){
//    if(i==7){
//      koreksi=(double)1/60;
//    }
//    else if(i==4){
//      koreksi=(double)3/60;
//    }else koreksi=(double)2/60;
    
  double   stime   = waktuSholat(i)+koreksi;
  stimeFloat[i] = stime;
  stimeInt[i][0] = floor(stime);
  stimeInt[i][1] = floor((stime-(double)stimeInt[i][0])*60);
  stimeInt[i][2] = floor((stime-(double)stimeInt[i][0]-(double)stimeInt[i][1]/60)*3600);
  }
  for(int i=0; i<=7; i++){
    char  out[21];
    sprintf(out, "sholat %01d %02d : %02d : %02d", i, stimeInt[i][0], stimeInt[i][1], stimeInt[i][2]);
    Serial.println(out);
    
  }
//  sprintf(dispNJadwal, " Shubuh %02d:%02d  Dzuhur %02d:%02d ",stimeInt[1][0], stimeInt[1][1],stimeInt[4][0], stimeInt[4][1]);
}



// -------------------------------------------
// Membuat file config JWS JSON di File Sistem

void membuatDataAwal() {

  String dataawal = "{\"dispiqomah\":0,\"iqmhs\":\"12\",\"iqmhd\":\"8\",\"iqmha\":\"6\",\"iqmhm\":\"5\",\"iqmhi\":\"5\",\"durasiadzan\":\"4\",\"ihti\":\"2\",\"latitude\":\"-6.65\",\"longitude\":\"115.22\",\"zonawaktu\":\"8\",\"hilal\":\"2\",\"koreksihjr\":\"0\",\"dispimsyak\":0,\"ringimsyak\":0,\"disppuasa\":1,\"dispiden\":1,\"dispdhuha\":0,\"namamasjid\":\"Mushola Al-Furqon\",\"informasi\":\"belum diisi\",\"ssid\":\"JWS Al-Furqon\",\"password\":\"alfurqon\",\"durasikutbah\":45}";

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, dataawal);

  File configFileJws = LittleFS.open(fileconfigjws, "w");
  
  if (!configFileJws) {
    Serial.println("Gagal membuat file configjws.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, configFileJws);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileJws.close();
    Serial.println("Berhasil membuat data awal file configjws.json");
  
  }  


//======================================================================================================================================================
  dataawal = "{\"cerah\":\"50\"}";
  error = deserializeJson(doc, dataawal);

  File configFileDisp = LittleFS.open(fileconfigdisp, "w");
  
  if (!configFileDisp) {
    Serial.println("Gagal membuat file configdisp.json untuk ditulis mungkin partisi belum dibuat");
    return;
  }
  
  serializeJson(doc, configFileDisp);

  if (error) {
    
    Serial.print(F("deserializeJson() gagal kode sebagai berikut: "));
    Serial.println(error.c_str());
    return;
    
  } else {
    
    configFileDisp.close();
    Serial.println("Berhasil membuat file configdisp.json");
  
  } 

}



// -------------------------------------------
// Membaca file config JWS JSON di File Sistem

void loadJwsConfig(const char *fileconfigjws, ConfigJws &configjws) {  

  File configFileJws = LittleFS.open(fileconfigjws, "r");

  if (!configFileJws) {
    
    Serial.println("Gagal membuka file configjws.json untuk dibaca");
    membuatDataAwal();
    Serial.println("Sistem restart...");
    ESP.restart();
    
  }

  size_t size = configFileJws.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFileJws.readBytes(buf.get(), size);

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, buf.get());  

  if (error) {
    Serial.println("Gagal parse fileconfigjws");
    return;
  }
  configjws.dispiqomah = doc["dispiqomah"];
  configjws.iqmhs = doc["iqmhs"];
  configjws.iqmhd = doc["iqmhd"];
  configjws.iqmha = doc["iqmha"];
  configjws.iqmhm = doc["iqmhm"];
  configjws.iqmhi = doc["iqmhi"];
  configjws.durasiadzan = doc["durasiadzan"];
  configjws.ihti = doc["ihti"];
  configjws.latitude = doc["latitude"];
  configjws.longitude = doc["longitude"];
  configjws.zonawaktu = doc["zonawaktu"];
  configjws.hilal = doc["hilal"];
//  configjws.cerah = doc["cerah"];
  configjws.koreksihjr = doc["koreksihjr"];
  configjws.dispimsyak = doc["dispimsyak"];
  configjws.ringimsyak = doc["ringimsyak"];
  configjws.disppuasa = doc["disppuasa"];
  configjws.dispiden = doc["dispiden"];
  configjws.dispdhuha = doc["dispdhuha"];
  strlcpy(configjws.namamasjid, doc["namamasjid"] | "", sizeof(configjws.namamasjid));
  strlcpy(configjws.informasi, doc["informasi"] | "", sizeof(configjws.informasi));
  strlcpy(configjws.ssid, doc["ssid"] | "", sizeof(configjws.ssid));
  strlcpy(configjws.password, doc["password"] | "", sizeof(configjws.password));
  configjws.durasikutbah = doc["durasikutbah"];

  configFileJws.close(); 

}

//void loadDispConfig(const char *fileconfigdisp, ConfigDisp &configdisp) {
//
//  File configFileDisp = LittleFS.open(fileconfigdisp, "r");
//
//  if (!configFileDisp) {
//    Serial.println("Gagal membuka fileconfigdisp untuk dibaca");
//    return;
//  }
//
//  size_t size = configFileDisp.size();
//  std::unique_ptr<char[]> buf(new char[size]);
//  configFileDisp.readBytes(buf.get(), size);
//  
//
//  DynamicJsonDocument doc(1024);
//  DeserializationError error = deserializeJson(doc, buf.get());
//
//  if (error) {
//    Serial.println("Gagal parse fileconfigdisp");
//    return;
//  }
//  
//  configdisp.cerah = doc["cerah"];
//
//  configFileDisp.close();
//  Disp.setBrightness(configdisp.cerah);
//
//}


// -------------------------------------------
// MEMBACA PARAMETER YANG TERSIMPAN

void bacaParameter() {

  Serial.println(" ");
  Serial.println("PARAMETER TERSIMPAN");
  Serial.print("Tampilkan Iqomah            : "); Serial.println(configjws.dispiqomah);
  Serial.print("Iqomah Subuh                : "); Serial.println(configjws.iqmhs);
  Serial.print("Iqomah Dzuhur               : "); Serial.println(configjws.iqmhd);
  Serial.print("Iqomah Ashar                : "); Serial.println(configjws.iqmha);
  Serial.print("Iqomah Maghrib              : "); Serial.println(configjws.iqmhm);
  Serial.print("Iqomah Isya                 : "); Serial.println(configjws.iqmhi);
  Serial.print("Durasi Adzan                : "); Serial.println(configjws.durasiadzan);
  Serial.print("Ihtiyati                    : "); Serial.println(configjws.ihti);
  Serial.print("Latitude                    : "); Serial.println(configjws.latitude, 6);
  Serial.print("Longitude                   : "); Serial.println(configjws.longitude, 6);
  Serial.print("Zona Waktu                  : "); Serial.println(configjws.zonawaktu);
  Serial.print("Derajat Hilal               : "); Serial.println(configjws.hilal);
//  Serial.print("Kecerahan                   : "); Serial.println(configjws.cerah);
  Serial.print("Koreksi Hijriah             : "); Serial.println(configjws.koreksihjr);
  Serial.print("Tampilkan Imsyak            : "); Serial.println(configjws.dispimsyak);
  Serial.print("Bunyikan alarm Imsyak       : "); Serial.println(configjws.ringimsyak);
  Serial.print("Tampilkan Pengingat Puasa   : "); Serial.println(configjws.disppuasa);
  Serial.print("Tampilkan Keterangan iden   : "); Serial.println(configjws.dispiden);
  Serial.print("Tampilkan Jadwal Dhuha      : "); Serial.println(configjws.dispdhuha);  
  Serial.print("Nama Masjid                 : "); Serial.println(configjws.namamasjid);
  Serial.print("Informasi                   : "); Serial.println(configjws.informasi);
  Serial.print("SSID                        : "); Serial.println(configjws.ssid);
  Serial.print("durasi kutbah               : "); Serial.println(configjws.durasikutbah);
  Serial.print("password                    : "); Serial.println(configjws.password);
  Serial.println(" ");
  
}


// PENGHITUNG JADWAL SHOLAT

void JadwalSholat() {

sholatCal(rTah, rBul, rTgl, configjws.zonawaktu, configjws.latitude, configjws.longitude, 45);
}



// ------------------------------
// UPDATE HISAB TANGGAL

F1kM_Hisab HisabTanggal;

int Hjr_Date, Hjr_Month, Hjr_Year;

void TanggalHijriah() {
  HisabTanggal.setLocationOnEarth(configjws.latitude, configjws.longitude); //Latitude Longitude TANGERANG
  HisabTanggal.setHilalVisibilityFactor(configjws.hilal);
  uint8_t tgl2H=rTgl;
  uint8_t bulan2H=rBul;
  uint16_t tahun2H=rTah;
  uint8_t Day2H=rHar;
  
//  Serial.println(Hjr_Day);
  
  if(floatnow>stimeFloat[6]){
    Serial.println("masuk");
    nextDay(rJam,tgl2H,bulan2H,tahun2H,tgl2H,bulan2H,tahun2H,Day2H);
    HisabTanggal.Greg2HijrDate(tgl2H,bulan2H,tahun2H,Hjr_Date,Hjr_Month,Hjr_Year);
    Hjr_Hari = Day2H;
  }else{
    HisabTanggal.Greg2HijrDate(tgl2H,bulan2H,tahun2H,Hjr_Date,Hjr_Month,Hjr_Year);
    Hjr_Hari = rHar + 1;
  }
//  Serial.println(Hjr_Day);
  

}



//-----------------------------------
// UPDATE WAKTU

void UpdateWaktu() {

  static long pM;
  static uint16_t d;
  static uint8_t lastTgl;
  static uint8_t lastMnt;
  
  BacaRTC();
  if(lastTgl!=rTgl){
    updateJWS();
    TanggalHijriah();
    lastTgl=rTgl;
  }
//  if(rMen!=lastMnt){
//    TanggalHijriah();
//    lastMnt=rMen;
//    Serial.println("update Hijriah");
//  }
  
  

//  if (millis() - pM > 1000) {
//    d++;
//    if (d < 2) {
//      Serial.println('t');
//      
////      JadwalSholat();
//      
//    } else if (d < 60) { // Update setiap 60 detik
//      rDet++;
//      if (rDet > 59) {
//        rMen++;
//        rDet=0;
//        if (rMen > 59) {
//          rJam++;
//          rMen = 0;
//        }
//      }
//    } else {
//      d = 0; 
//    }
//    pM = millis();    
//  }
  
}



//----------------------------------------------------------------------
// XML UNTUK JEMBATAN DATA MESIN DENGAN WEB

String XMLWaktu(){
  String Json = "{\"rTah\":";
  Json += rTah;
  Json += ",\"rBul\":";
  Json += rBul;
  Json += ",\"rTgl\":";
  Json += rTgl;
  Json += ",\"rJam\":";
//    if(rJam<10){
//      Json+="0";
      Json+=rJam;
//    }else Json+=rJam;
  Json += ",\"rMen\":";
//    if(rMen<10){
//      Json+="0";
      Json+=rMen;
//    }else Json+=rMen;
  Json += ",\"rDet\":";
//    if(rDet<10){
//      Json+="0";
      Json+=rDet;
//    }else Json+=rDet;
  Json += ",\"rSuhu\":";
  Json += celsius - 5;
  Json += "}";
  return Json;
}

String XMLDataJWS(){

  String Json = "{\"dispiqomah\":";
  Json += configjws.dispiqomah;
  Json += ",\"IqomahSubuh\":";
  Json += configjws.iqmhs;
  Json += ",\"IqomahDzuhur\":";
  Json += configjws.iqmhd;
  Json += ",\"IqomahAshar\":";
  Json += configjws.iqmha;
  Json += ",\"IqomahMaghrib\":";
  Json += configjws.iqmhm;
  Json += ",\"IqomahIsya\":";
  Json += configjws.iqmhi;
  Json += ",\"DurasiAdzan\":";
  Json += configjws.durasiadzan;
  Json += ",\"Ihtiyati\":";
  Json += configjws.ihti;
  Json += ",\"Latitude\":";
  Json += String(configjws.latitude, 6);//configjws.latitude;
  Json += ",\"Longitude\":";
  Json += String(configjws.longitude, 6);
  Json += ",\"ZonaWaktu\":";
  Json += configjws.zonawaktu;
  Json += ",\"Hilal\":";
  Json += configjws.hilal;
  Json += ",\"koreksihjr\":";
  Json += configjws.koreksihjr;
  Json += ",\"dispimsyak\":";
  Json += configjws.dispimsyak;
  Json += ",\"ringimsyak\":";
  Json += configjws.ringimsyak;
  Json += ",\"disppuasa\":";
  Json += configjws.disppuasa;
  Json += ",\"dispiden\":";
  Json += configjws.dispiden;
  Json += ",\"dispdhuha\":";
  Json += configjws.dispdhuha;  
  Json += ",\"NamaMasjid\":\"";
  Json += String(configjws.namamasjid) + "\"";
  Json += ",\"informasi\":\"";
  Json += String(configjws.informasi)+"\"";
  Json += ",\"ssid\":\"";
  Json += String(configjws.ssid) + "\"";
  Json += ",\"password\":\"";
  Json += String(configjws.password)+"\"";
  Json += ",\"durasikutbah\":";
  Json += configjws.durasikutbah;
  Json += ",\"wsim\":\"";
  Json += String(stimeInt[0][0])+":"+String(stimeInt[0][1])+":"+String(stimeInt[0][2])+"\"";
  Json += ",\"wssh\":\"";
  Json += String(stimeInt[1][0])+":"+String(stimeInt[1][1])+":"+String(stimeInt[1][2])+"\"";
  Json += ",\"wstr\":\"";
  Json += String(stimeInt[2][0])+":"+String(stimeInt[2][1])+":"+String(stimeInt[2][2])+"\"";
  Json += ",\"wsdh\":\"";
  Json += String(stimeInt[3][0])+":"+String(stimeInt[3][1])+":"+String(stimeInt[3][2])+"\"";
  Json += ",\"wsdz\":\"";
  Json += String(stimeInt[4][0])+":"+String(stimeInt[4][1])+":"+String(stimeInt[4][2])+"\"";
  Json += ",\"wsas\":\"";
  Json += String(stimeInt[5][0])+":"+String(stimeInt[5][1])+":"+String(stimeInt[5][2])+"\"";
  Json += ",\"wsma\":\"";
  Json += String(stimeInt[6][0])+":"+String(stimeInt[6][1])+":"+String(stimeInt[6][2])+"\"";
  Json += ",\"wsis\":\"";
  Json += String(stimeInt[7][0])+":"+String(stimeInt[7][1])+":"+String(stimeInt[7][2])+"\"";
  Json += "}";
  return Json;

}
