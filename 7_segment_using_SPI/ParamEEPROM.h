/*------------------------------------------
// Function Setup Parameter EEPROM
------------------------------------------*/
/* Struktur Data EEPROM 
 *      1   byte add  0   unit8_t       Parameter State  212 if ok    
 *      4   byte add  1   float         Latitude         code NLA
 *      4   byte add  5   float         Longitude        code NLO
 *      4   byte add  9   float         Altitude         code NAL   
 *      4   byte add 13   float         TimeZone         code NTZ
 *      1   byte add 17   uint8_t       Masjid Type      code NMT
 *      1   byte add 18   uint8_t       Led Brighnest    code NBL
 *      1   byte add 19   uint8_t       Ihtiyati Time    code NIH  -- waktu pengaman
 *      1   byte add 20   uint8_t       Sholat Time      code NSO  -- waktu tunggu sholat
 *      1   byte add 21   uint8_t       Jum'at Time      code NJM  -- waktu tunggu khutbah jumat
 *      1   byte add 22   uint8_t       IQ Subuh         code NI1  -- waktu iqomah subuh
 *      1   byte add 23   uint8_t       IQ Dhuhur        code NI4  -- waktu iqomah Dhuhur
 *      1   byte add 24   uint8_t       IQ Ashar         code NI5  -- waktu iqomah Ashar
 *      1   byte add 25   uint8_t       IQ Maghrib       code NI6  -- waktu iqomah Magrib
 *      1   byte add 26   uint8_t       IQ Isya          code NI7  -- waktu iqomah Isya
 *      1   byte add 27   uint8_t       Buzzer           code NBZ  -- Buzzer on/off
 *      1   byte add 28   uint8_t       Show Imsak       code NSI  -- Imsak on/off
 *      1   byte add 29   uint8_t       Show Terbit      code NST  -- Terbit on/off
 *      1   byte add 30   uint8_t       Show Dhuha       code NSU  -- Dhuha on/off
 *      1   byte add 31   int8_t        Corection HijrD  code NCH  -- Corection Hijriah Date
 *    
 *      26   byte add 29   -------  EMPTY  --------
 *      50  byte add 55   char          Masjid Name      code CMN    
 *      150 byte add 130  char          Info 1           code CN1
 *      150 byte add 280  char          Info 2           code CN2
 *      150 byte add 430  char          Info 3           code CN3
 *      150 byte add 580  char          Sholat Message   code CSM  -- pesan mejelang sholat biasa
 *      150 byte add 730  char          Jumat Message    code CJM  -- pesan setelah azan 
 *      144 byte add 880  -------  EMPTY  --------
 *  
 *      Memory Size arduino Uno
 *        Flash  32k bytes (of which .5k is used for the bootloader)
 *        SRAM   2k bytes
 *        EEPROM 1k byte
 */
//#include "Ali.h"

//Structure of Variable 
typedef struct  // loaded to EEPROM
  {
    uint8_t state;   // 1 byte  add 0
    float   L_LA;    // 4 byte  add 1
    float   L_LO;    // 4 byte  add 5
    float   L_AL;    // 4 byte  add 9
    float   L_TZ;    // 4 byte  add 13
    uint8_t MT;      // 1 byte  add 17  // value 1-masjid  2-mushollah 3-surau 4-langgar 
    uint8_t BL;      // 1 byte  add 18
    uint8_t IH;      // 1 byte  add 19
    uint8_t SO;      // 1 byte  add 20
    uint8_t JM;      // 1 byte  add 21
    uint8_t I1;      // 1 byte  add 22
    uint8_t I4;      // 1 byte  add 23
    uint8_t I5;      // 1 byte  add 24
    uint8_t I6;      // 1 byte  add 25
    uint8_t I7;      // 1 byte  add 26
    uint8_t BZ;      // 1 byte  add 27
    uint8_t SI;      // 1 byte  add 28
    uint8_t ST;      // 1 byte  add 29
    uint8_t SU;      // 1 byte  add 30
    int8_t  CH;      // 1 byte  add 31
  } struct_param;

typedef struct  
  { 
    uint8_t   hD;
    uint8_t   hM;
    uint16_t  hY;
  } hijir_date;

   
// Variable by Structure     
struct_param    Prm;

//Blue tooth Pram Receive
char        CH_Prm[155];

uint8_t vol = 15;

void LoadPrm();
void GetPrm();
void SendPrm();
void set_default_prm();
void set_default_time();
void waktuSholatNow();
void UpdateWaktu();

void serialEvent()
  { 
    int prm_idx = 0;
    char bchar;

    bchar =Serial.peek();
    
    if ((bchar == 'C') or (bchar == 'N') or (bchar == 'S'))
      {
        while ((bchar != '\n')and(prm_idx < 149))
          {
          if(Serial.available())
            { 
              bchar = (char)Serial.read();
              CH_Prm[prm_idx]=bchar;
              prm_idx ++;
            }
          }
        CH_Prm[prm_idx-1]='\0';
        LoadPrm();
      }
    else
      {  
        while(Serial.available()) {Serial.read();}
      }
  }
  
void LoadPrm()
  { 
    String BT_Param;
    uint16_t  eeAdd = 0;
    uint8_t   eePut = 0; 
    uint8_t   eeMax = 0;
    uint8_t   lenprm = strlen(CH_Prm)-3;

    // Put Char Data 
    if (CH_Prm[0]=='C')
      { 
        Serial.println("masuk sini");
        if(CH_Prm[1]=='M' and CH_Prm[2]=='N')       {eeAdd = 55;  eeMax=74;}
        else if(CH_Prm[1]=='N' and CH_Prm[2]=='1')  {eeAdd = 130; eeMax=149;} 
        else if(CH_Prm[1]=='N' and CH_Prm[2]=='2')  {eeAdd = 280; eeMax=149;}  
        else if(CH_Prm[1]=='N' and CH_Prm[2]=='3')  {eeAdd = 430; eeMax=149;}
        else if(CH_Prm[1]=='S' and CH_Prm[2]=='M')  {eeAdd = 580; eeMax=149;}
        else if(CH_Prm[1]=='J' and CH_Prm[2]=='M')  {eeAdd = 730; eeMax=149;}
        else if(CH_Prm[1]=='O'){Serial.println("bluetooth tersambung"); return;}

        int eeCount =0;        
        while((eeCount<= lenprm) and (eeCount < eeMax))
          {
            EEPROM.update(eeAdd+eeCount,CH_Prm[3+eeCount]);
            eeCount++;
          }
        EEPROM.update(eeAdd+eeCount,'\0');  
      }
      
    // Put Numeric Data
    else if (CH_Prm[0] =='N')
      {
        BT_Param = String(CH_Prm);
        
        
        if(BT_Param.substring(1,3).equals("LA"))       {eeAdd = 1 ; eePut=1;} 
        else if(BT_Param.substring(1,3).equals("LO"))  {eeAdd = 5 ; eePut=1;}  
        else if(BT_Param.substring(1,3).equals("AL"))  {eeAdd = 9 ; eePut=1;}  
        else if(BT_Param.substring(1,3).equals("TZ"))  {eeAdd = 13; eePut=1;}
        else if(BT_Param.substring(1,3).equals("MT"))  {eeAdd = 17; eePut=2;}
        else if(BT_Param.substring(1,3).equals("BL"))  {eeAdd = 18; eePut=2;}
        else if(BT_Param.substring(1,3).equals("IH"))  {eeAdd = 19; eePut=2;}
        else if(BT_Param.substring(1,3).equals("SO"))  {eeAdd = 20; eePut=2;}
        else if(BT_Param.substring(1,3).equals("JM"))  {eeAdd = 21; eePut=2;}
        else if(BT_Param.substring(1,3).equals("I1"))  {eeAdd = 22; eePut=2;}
        else if(BT_Param.substring(1,3).equals("I4"))  {eeAdd = 23; eePut=2;}
        else if(BT_Param.substring(1,3).equals("I5"))  {eeAdd = 24; eePut=2;}
        else if(BT_Param.substring(1,3).equals("I6"))  {eeAdd = 25; eePut=2;}
        else if(BT_Param.substring(1,3).equals("I7"))  {eeAdd = 26; eePut=2;}
        else if(BT_Param.substring(1,3).equals("BZ"))  {eeAdd = 27; eePut=2;}                
        else if(BT_Param.substring(1,3).equals("SI"))  {eeAdd = 28; eePut=2;}                
        else if(BT_Param.substring(1,3).equals("ST"))  {eeAdd = 29; eePut=2;}                
        else if(BT_Param.substring(1,3).equals("SU"))  {eeAdd = 30; eePut=2;}                
        else if(BT_Param.substring(1,3).equals("CH"))  {eeAdd = 31; eePut=3;}                
                
        

        if(eePut == 1) // put float
          {
          EEPROM.put(eeAdd,BT_Param.substring(3,BT_Param.length()).toFloat());
          Serial.println("put 1");
          }
        if(eePut == 2) // put uint8_t
          {
          EEPROM.put(eeAdd,(uint8_t)BT_Param.substring(3,BT_Param.length()).toInt());
          Serial.println("put 2");
          }
        if(eePut == 3) // put uint8_t
          {
          EEPROM.put(eeAdd,(int8_t)BT_Param.substring(3,BT_Param.length()).toInt());
          Serial.println("put 3");
          }
  
  
      }
    else
    if (CH_Prm[0]=='S' and CH_Prm[1]=='D' and CH_Prm[2]=='T')  
      {
          BT_Param = String(CH_Prm);
          uint16_t year = byte(BT_Param.substring(7,9).toInt());
          uint8_t month = byte(BT_Param.substring(5,7).toInt());
          uint8_t day = byte(BT_Param.substring(3,5).toInt());
          uint8_t jam = byte(BT_Param.substring(9,11).toInt());
          uint8_t menit = byte(BT_Param.substring(11,13).toInt());
          uint8_t detik = byte(BT_Param.substring(13,15).toInt());
          Rtc.SetDateTime(RtcDateTime(year, month, day, jam, menit, detik));
          
//          Clock.setClockMode(false);
//          Clock.setDate(byte(BT_Param.substring(3,5).toInt()));
//          Clock.setMonth(byte(BT_Param.substring(5,7).toInt()));
//          Clock.setYear(byte(BT_Param.substring(7,9).toInt()));
//          Clock.setHour(byte(BT_Param.substring(9,11).toInt()));
//          Clock.setMinute(byte(BT_Param.substring(11,13).toInt()));
//          Clock.setSecond(byte(BT_Param.substring(13,15).toInt()));       
//          Clock.setDoW(byte(BT_Param.substring(15,16).toInt()));
      }
   
    // Get New/updeted parameter*/
    GetPrm();
    myDFPlayer.volume(vol);
    UpdateWaktu();
    waktuSholatNow();
    if (CH_Prm[0]=='S' and CH_Prm[1]=='Q' and CH_Prm[2]=='P')
      {
        SendPrm();    
      }
      SendPrm(); 

  }
  
void GetPrm()
  {
//    Serial.println("Get parameter");
    // Get Parameter and check
    EEPROM.get(0, Prm);
//    Serial.println(Prm.state);
    if(Prm.state != 1) //check value of Prm State .. set Default  
      {
        set_default_prm();
        Serial.println("default");
        
      }
    if (rTah < 2021)//check date time .. less than 1 jan 2018 set Default
      {
        set_default_time();
      }
    vol = map(Prm.BL, 0, 200, 0, 30);
    
    SendPrm();
    
//    Iqomah[1]=Prm.I1;
//    Iqomah[4]=Prm.I4;
//    Iqomah[5]=Prm.I5;
//    Iqomah[6]=Prm.I6;
//    Iqomah[7]=Prm.I7;
    // user New Parameter
//    setBrightness(int(Prm.BL));
//    update_All_data();
   
  }

void set_default_prm()
      {
        // Put Parameter start form addr 500
        Prm = (struct_param){1,-6.39524,106.80256,45,7,1,20,2,10,30,15,10,10,7,10,1,1,1,1,0};
        EEPROM.put(0,Prm);
        EEPROM.put(55, "Masjid Al-Arsy\0");
        EEPROM.put(130, "Info 1\0");
        EEPROM.put(280,"Info 2\0");
        EEPROM.put(430,"Info 3\0");
        EEPROM.put(580,"Ayo Sholat\0");
        EEPROM.put(730,"Ayo Jumatan\0");

      }

void set_default_time()
  {
      Rtc.SetDateTime(RtcDateTime(2021, 04, 29, 10, 21, 0));
//    Clock.setClockMode(false);  // set to 24h
//    Clock.setYear(byte(18));
//    Clock.setMonth(byte(1));
//    Clock.setDate(byte(1));
//    Clock.setDoW(byte(2));
//    Clock.setHour(byte(12));
//    Clock.setMinute(byte(0));
//    Clock.setSecond(byte(0));
//    Clock.turnOffAlarm(1);
//    Clock.turnOffAlarm(2);    
  }

// for debug
void SendPrm()
  {
   // char Info1[150];
  //  Serial.println(sizeof(Prm));
  //  Serial.print(F("state\t: "));  Serial.println(Prm.state);
    
    Serial.print(F("NLA  : ")); Serial.print(Prm.L_LA,7);Serial.print("\t");
    Serial.print(F("NLO  : ")); Serial.print(Prm.L_LO,7);Serial.print("\n");
    Serial.print(F("NAL  : ")); Serial.print(Prm.L_AL,7);Serial.print("\t");
    Serial.print(F("NTZ  : ")); Serial.print(Prm.L_TZ,0);Serial.print("\n");
    Serial.print(F("NMT  : ")); Serial.print(Prm.MT);Serial.print("\t");
    Serial.print(F("NIH  : ")); Serial.print(Prm.IH);Serial.print("\t");
    Serial.print(F("NCH  : ")); Serial.print(Prm.CH);Serial.print("\n");
    Serial.print(F("NSO  : ")); Serial.print(Prm.SO);Serial.print("\t");
    Serial.print(F("NJM  : ")); Serial.print(Prm.JM);Serial.print("\n");
    Serial.print(F("NI1  : ")); Serial.print(Prm.I1);Serial.print("\t");
    Serial.print(F("NI4  : ")); Serial.print(Prm.I4);Serial.print("\t");
    Serial.print(F("NI5  : ")); Serial.print(Prm.I5);Serial.print("\n");
    Serial.print(F("NI6  : ")); Serial.print(Prm.I6);Serial.print("\t");
    Serial.print(F("NI7  : ")); Serial.print(Prm.I7);Serial.print("\t");
    Serial.print(F("NBZ  : ")); Serial.print(Prm.BZ);Serial.print("\n");
    Serial.print(F("NSI  : ")); Serial.print(Prm.SI);Serial.print("\t");
    Serial.print(F("NST  : ")); Serial.print(Prm.ST);Serial.print("\t");
    Serial.print(F("NSU  : ")); Serial.print(Prm.SU);Serial.print("\n");
    Serial.print(F("NBL  : ")); Serial.print(Prm.BL);Serial.print("\n");
    Serial.print(F("VOL  : ")); Serial.print(vol);Serial.print("\n");
//    Serial.print(drawInfo(55));Serial.print("\n");
//    Serial.print(drawInfo(130));Serial.print("\n");
//    Serial.print(drawInfo(280));Serial.print("\n");
//    Serial.print(drawInfo(430));Serial.print("\n");
//    Serial.print(drawInfo(580));Serial.print("\n");
//    Serial.print(drawInfo(730));Serial.print("\n");
//    Serial.print(F("=== end of parameters ===\n"));

  }
