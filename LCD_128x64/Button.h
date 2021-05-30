//void tombol(){
//  static bool lastButtonState;
//  static bool buttonState;
//  static uint32_t lastDebounceTime;
//  uint32_t tmr = millis();
//  static bool reading;
//  static bool rst;
//  
//  int sensorValue = analogRead(A0);
//  if(sensorValue<200){
//    reading = true;
//  }else{
//    reading = false;
//  }
//
//  if (!reading != lastButtonState) {
//    lastDebounceTime = tmr;
//  }
////  Serial.println(tmr - lastDebounceTime);
//  if ((tmr - lastDebounceTime) > 50) {
//    if (!reading != buttonState) {
//      buttonState = !reading;
//      if((tmr - lastDebounceTime)>100){
//        return;
//      }else{
//        if (buttonState == HIGH) {
//          Disp.clear();
//          if(tmputama!=2){
//            
//            tmputama = 2;
//            waktuSholatNow();
////            switch(SholatNow){
////              case 1:
////                menitiqmh = configjws.iqmhs;
////                break;
////              case 4:
////                menitiqmh = configjws.iqmhd;
////                break;
////              case 5:
////                menitiqmh = configjws.iqmha;
////                break;
////              case 6:
////                menitiqmh = configjws.iqmhm;
////                break;
////              case 7:
////                menitiqmh = configjws.iqmhi;
////                break;
////              default:
////                menitiqmh = 0;
////                break;        
////            }
//
//          }
//          else{
//            Iqomah(false);
//            if(azzan){
//              tmputama = 1;
//            }else
//            tmputama = 0;
//          }
//          
//          Serial.println(millis() - lastDebounceTime);
//        }
//      }
//      
//    }
//  }
//  
//
//  lastButtonState = !reading;
//}

void tombolCap(){
  int sensorValue = analogRead(A0);
  static bool buttonState;
  static bool reading;
  static bool st;

  if(sensorValue<200){
    reading = false;
  }else{
    reading = true;
  }
  if(SholatNow==0){
    return;
  }


  if(buttonState != reading){
    buttonState = reading;
//    digitalWrite(2, HIGH);
    if(buttonState==HIGH){
      Serial.println("trg");
//      runAdzanSingle(tmputama, true);
      if(tmputama!=2){
//        Disp.clear();
        
        tmputama = 2;
        menitiqomah = 0;
        switch(SholatNow){
          case 1:
            menitiqomah = configjws.iqmhs;
            break;
          case 4:
            menitiqomah = configjws.iqmhd;
            break;
          case 5:
            menitiqomah = configjws.iqmha;
            break;
          case 6:
            menitiqomah = configjws.iqmhm;
            break;
          case 7:
            menitiqomah = configjws.iqmhi;
            break;
          default:
            menitiqomah = 0;
            break;        
        }
        
        if(menitiqomah==0){
          
          tmputama = 0;
        }else{
          menitiqomah=menitiqomah-1;
//          detikiqmh=60;
//          Disp.clear();//sebelumnya ada
//          Iqomah(false);
        }
      }
      else{
//        Disp.clear();//sebelumnya ada
//        Iqomah(false);
        tmputama = 0;
      }
      
    }
  }
  
}
