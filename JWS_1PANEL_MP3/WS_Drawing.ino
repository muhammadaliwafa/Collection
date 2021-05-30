// =========================================
// Drawing Content Block====================    
// =========================================

void drawAzzan(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return;
    uint8_t           ct_limit =40;  //harus angka genap
    static uint8_t    ct;
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();

    if((Tmr-lsRn) > 500 and ct <= ct_limit)
      {
        lsRn = Tmr;
        if((ct%2) == 0)
          { //Disp.drawRect(1,2,62,13);
            fType(0);
            dwCtr(0,0,"ADZAN");
            fType(1);
            if(jumat) {dwCtr(0,8,sholatN(8));}
            else      {dwCtr(0,8,sholatN(SholatNow));}
            Buzzer(1);
          }
        else 
          { Buzzer(0);}
        DoSwap = true; 
        ct++;
      }
    if ((Tmr-lsRn)>2000 and (ct > ct_limit))
      {dwDone(DrawAdd);
       ct = 0;
       Buzzer(0);}
  }

void drawIqomah(int DrawAdd)  // Countdown Iqomah (9 menit)
  {  
    // check RunSelector
    if(!dwDo(DrawAdd)) return;

    static uint16_t   lsRn;
    uint16_t          Tmr = millis();
    static int        ct;
    int               mnt, scd,cn_l ;
    char              locBuff[6];           
    
    cn_l  = (Iqomah[SholatNow]*60);
    //Disp.drawRect(1,2,62,13);
    if((Tmr-lsRn) > 1000 and ct <=cn_l)
      {
          lsRn = Tmr;        
          mnt = floor((cn_l-ct)/60); 
          scd = (cn_l-ct)%60;
          if(mnt>0) {sprintf(locBuff,"%02d:%02d",mnt,scd);}
            else    {sprintf(locBuff,"%02d",scd);}
          if((ct%2) == 0){
          fType(0); //font iqomah
          dwCtr(0,-1,"IQOMAH");}          
          fType(0);
          dwCtr(0,8,locBuff);
          if (ct> (cn_l-10)) Buzzer(1);   // Buzzer on 2 seccon before Iqomah
          ct++;
          DoSwap = true;
             
      }
    if (ct > cn_l)
      {
       dwDone(DrawAdd);
       ct = 0;
       Buzzer(0);
      }    
  }

void drawSholat_S(int sNum,int c) // Box Sholah Time   tampilan jadwal sholat
  {
    char  BuffTime[10];
    char  BuffShol[7];
    float   stime   = sholatT[sNum];
    uint8_t shour   = floor(stime);
    uint8_t sminute = floor((stime-(float)shour)*60);
    uint8_t ssecond = floor((stime-(float)shour-(float)sminute/60)*3600);
    sprintf(BuffTime,"%02d:%02d",shour,sminute);
   // Disp.drawRect(c+1,2,62,13);
    fType(1); dwCtr(c,0,sholatN(sNum)); //tulisan waktu sholat
    fType(1); dwCtr(c,9,BuffTime);   //jadwal sholatnya
    DoSwap = true;          
  }

void drawSholat(int DrawAdd)
  {
    // check RunSelector
//    int DrawAdd = 0b0000000000000100;
    if(!dwDo(DrawAdd)) return; 

    static uint8_t    x;
    static uint8_t    s; // 0=in, 1=out
    static uint8_t    sNum; 
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();
    uint8_t           c=0;
    uint8_t    first_sNum = 0; 
    int               DrawWd=DWidth - c;    

    if((Tmr-lsRn)>10) 
      {
        if(s==0 and x<(DrawWd/2)){x++;lsRn=Tmr;}
        if(s==1 and x>0){x--;lsRn=Tmr;}
      }
      
    if((Tmr-lsRn)>2000 and x ==(DrawWd/2)) {s=1;}
    if (x == 0 and s==1) 
      { 
        if (sNum <7){sNum++;}
        else 
          { 
           dwDone(DrawAdd);
           sNum=0;
          } 
        s=0;
      }

    if(Prm.SI==0) {first_sNum =1;}
    else {first_sNum =0;}
    if(Prm.SI==0 and sNum == 0) {sNum=1;}
    if(Prm.ST==0 and sNum == 2) {sNum=3;}
    if(Prm.SU==0 and sNum == 3) {sNum=4;}

 
    if(  (((sNum == first_sNum) and s ==0) or 
          ((sNum == 7)and s == 1)) 
          and x <=20) {//drawSmallTS(int(x/2));
          } 
    else {//drawSmallTS(10);
    }
    drawSholat_S(sNum, c);

    Disp.drawFilledRect(c,0,c+DrawWd/2-x,15,0);
    Disp.drawFilledRect(DrawWd/2+x+c,0,63,15,0);
  }

void drawGreg_DS(uint16_t y)   //Draw Date
  { 
    char  Buff[20];
    sprintf(Buff,"%02d/%02d/%02d",now.day(),now.month(),now.year()-2000);
    dwCtr(0,y-2,Buff);
    DoSwap = true;
  }

void drawSmallTS(int x) //jam kecil  ketika sholat
  {
    char BuffH[3];
    char BuffM[3];
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();
    sprintf(BuffH,"%02d",now.hour());
    sprintf(BuffM,"%02d",now.minute());
    fType(0);
    Disp.drawText((x-10)+1,-1,BuffH);
    Disp.drawText((x-10)+1,8,BuffM);
   // if(Tmr-lsRn<500)Disp.drawLine((x-10)+1,7,(x-10)+9,7,1);
   // if(Tmr-lsRn>1000) lsRn = Tmr;
    DoSwap = true;
  }  

void drawGreg_TS(uint16_t y)   // Draw Time
  {
    char  Buff[20];
    //sprintf(Buff,"%02d:%02d:%02d",now.hour(),now.minute(),now.second());
    sprintf(Buff,"%02d:%02d",now.hour(),now.minute());
    dwCtr(0,y,Buff);
    DoSwap = true; 
  }
void drawGreg_cil(uint16_t y)   // Draw Time
  {
    char  Buff[20];
    sprintf(Buff,"%02d:%02d:%02d",now.hour(),now.minute(),now.second());    
    dwCtr(0,y,Buff);
    DoSwap = true; 
  }
void Jam_GD(uint16_t y)   // Draw Time Depan  jam besar di depan
  {
    char  BuffJ[6];
    char  BuffM[6];
    char  BuffD[6];
    sprintf(BuffJ,"%02d",now.hour());
    sprintf(BuffM,"%02d",now.minute());
  //  sprintf(BuffD,"%02d",now.second());
    fType(3);
    Disp.drawText(0,y,BuffJ);  //tampilkan jam
    Disp.drawText(19,y,BuffM);  //tampilkan menit
  //  fType(3);
  //  Disp.drawText(19,y,BuffD);  //tampilkan detik
    Disp.drawRect(15,y+3,16,y+5,1);
    Disp.drawRect(15,y+10,16,y+12,1);
    DoSwap = true; 
  }

void anim_DT(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return; 
    
    static uint8_t    y;
    static uint8_t    s; // 0=in, 1=out              
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();

    if((Tmr-lsRn)>100) 
      { 
        if(s==0 and y<17){lsRn=Tmr;y++;}
        if(s==1 and y>0){lsRn=Tmr;y--;}
      }
    if((Tmr-lsRn)>10000 and y ==17) {s=1;}
    if (y==7)
      {
       // Disp.drawRect(1,2,62,13);
      }
    if (y == 0 and s==1) {dwDone(DrawAdd); s=0;}
    fType(4);                           ////////////////////////////jam kuru dowoooooo
    drawGreg_cil(y-16);
  //  fType(2);
 //   drawGreg_DS(16-y);      
    }
 
void anim_JG(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return; 
    
    static uint8_t    y;
    static uint8_t    s; // 0=in, 1=out              
    static uint16_t   lsRn;
    uint16_t          Tmr = millis();

    if((Tmr-lsRn)>100) 
      { 
        if(s==0 and y<17){lsRn=Tmr;y++;}
        if(s==1 and y>0){lsRn=Tmr;y--;}
      }
    if((Tmr-lsRn)>10000 and y ==17) {s=1;}
    if (y==7)
      {
       // Disp.drawRect(1,2,62,13);
      }
    if (y == 0 and s==1) {dwDone(DrawAdd); s=0;}
    //fType(1);
    //drawGreg_TS(y-8);
    
    Jam_GD(17-y);      
    }
void dwMrq(const char* msg, int Speed, int dDT, int DrawAdd) //running teks ada jam nya
  { 
    // check RunSelector
    static uint16_t   x; 
    if(!dwDo(DrawAdd)) return;
    if (reset_x !=0) { x=0;reset_x = 0;}      

       
    static uint16_t   lsRn;
    int fullScroll = Disp.textWidth(msg) + DWidth;    
    uint16_t          Tmr = millis();
    if((Tmr-lsRn)> Speed)
      { lsRn=Tmr;
        if (x < fullScroll) {++x;}
        else {  dwDone(DrawAdd); 
                x = 0;return;}
     if(dDT==1)
        {
        fType(1);  //Marquee    jam yang tampil di bawah
        Disp.drawText(DWidth - x, 0, msg); //runing teks diatas
        fType(1);
        if (x<=6)                     { drawGreg_TS(16-x);}
        else if (x>=(fullScroll-6))   { drawGreg_TS(16-(fullScroll-x));}
        else                          { 
          //Disp.drawRect(1,8,30,8);//garis tengah
                                        drawGreg_TS(9);}//posisi jamnya yang bawah
        }
     else if(dDT==2) //jam yang diatas
        {    
        fType(1);
        if (x<=6)                     { drawGreg_TS(x-6);}
        else if (x>=(fullScroll-6))   { drawGreg_TS((fullScroll-x)-6);}
        else                          { 
          //Disp.drawRect(1,7,30,7);//garis tengah
                                        drawGreg_TS(0);}  //posisi jam nya yang diatas
        fType(1); //Marquee  running teks dibawah
        Disp.drawText(DWidth - x, 9 , msg);//runinng teks dibawah
        }
     else
        {
        fType(1);
    //    Disp.drawLine(1,2,62,2);  nampilkan garis
     //   Disp.drawLine(1,13,62,13); //nampilkan garis
     //   Disp.drawText(DWidth - x, 4, msg);
        }
        DoSwap = true; 
      }          
  }

void blinkBlock(int DrawAdd)
  {
    // check RunSelector
    if(!dwDo(DrawAdd)) return;

    static uint16_t   lsRn;
    static uint16_t   ct, ct_l;
    uint16_t          Tmr = millis();
    int               mnt, scd;//
    char              locBuff[6];//

    if(jumat)       {ct_l = Prm.JM * 60;}
    else            {ct_l = Prm.SO * 60;}
    jumat =false;
     
    if((Tmr-lsRn)> 1000)
      { lsRn=Tmr;
      
        //Disp.drawChar(1, 1 , ct);
        mnt = floor((ct_l-ct)/60);
        scd = (ct_l-ct)%60;
        sprintf(locBuff,"%d:%02d",mnt,scd);
        fType(2);
        Disp.drawText(1,7,locBuff); // tampil tunggu sholat
        if((ct%2) == 0)
          { 

            Disp.drawFilledRect(DWidth-3, DHeight-3, DWidth-2, DHeight-2);}
        DoSwap = true; 
        ct++;}
    if (ct> ct_l) 
      { dwDone(DrawAdd);
        azzan = false;
        ct = 0;
      }
   // Disp.drawText(1,7,String(ct_l-ct)); 
  }

// =========================================
// Drawing Tools============================
// =========================================
boolean dwDo(int DrawAdd)
  { if (RunSel== DrawAdd) {return true;}
    else return false;}
  
void dwDone(int DrawAdd)
  { RunFinish = DrawAdd;
    RunSel = 0;}
    
void dwCtr(int x, int y,const char* Msg)
  { int   tw = Disp.textWidth(Msg);
    int   th = Disp.textHeight();
    int   c = int((DWidth-x-tw)/2);
    Disp.drawFilledRect(x+c-1,y,x+tw+c,y+th,0);
    Disp.drawText(x+c,y,Msg);}

void Buzzer(uint8_t state)
  {
    if(state ==1 and Prm.BZ == 1)
      {tone(BUZZ, 500, 400);}
    else 
      {noTone(BUZZ);}
  }
  
void fType(int x)
  {
    if(x==0) Disp.setFont(Font0);
    else if(x==1) Disp.setFont(Font1); 
    else if(x==2) Disp.setFont(Font2);
    else if(x==3) Disp.setFont(Font3);
    else if(x==4) Disp.setFont(Font4);
    else Disp.setFont(Font5);  
  }

// digunakan untuk menghitung hari pasaran
  int jumlahhari(){ 
  DateTime  now = RTC.now(); 
  int d= now.day();
  int m= now.month();
  int y= now.year();
  int hb[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
  int ht = (y - 1970) * 365 - 1;
  int hs = hb[m - 1] + d;
  int kab = 0;
  int i;
  if(y % 4 == 0) {
    if(m > 2) {
    hs++;
    }
  }
  for(i = 1970; i < y; i++) {
    if(i % 4 == 0) {
    kab++;
    }
  }
  return (ht + hs + kab); 
}

