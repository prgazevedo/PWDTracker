
/*
 * Copyright 2018 Pedro Azevedo (prgazevedo@gmail.com)
 * and other contributors as indicated by the @author tags.
 **********************************************************************
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "_const.h"
#include "images.h"


int line=0;
void increaseLine(){
    if(line>40)line=0;
    else line+=10;
}


void OLED_clear(){
  display.clear();
}

void OLED_write(String toWrite){

    display.drawString(0, line, toWrite);
    increaseLine();
    //Serial.println("OLED_write called:"+toWrite);
    
    display.display(); //show
}

void OLED_write(int a, int b, String toWrite){
    display.drawString(a, b, toWrite);
    //Serial.println("OLED_write called:"+toWrite+ " at a:" +a+" b:"+b);
    display.display(); //show
}

void OLED_COMMS_DATA(){
  OLED_clear();
  if(_ROLE==0)OLED_write(0,0,"V1."+_SUBVERSION+"-Sender: ");
  else OLED_write(0,0,"V1."+_SUBVERSION+"-Receiver-"+SSID_NAME);
  OLED_write(0,10,"Packet Id:");
  OLED_write(60,10,String(pdata.ID));
  OLED_write(0,20,"TimeStamp:");
  OLED_write(60,20,String(pdata.timeMillis));
  OLED_write(0,30,"Latitude:");
  OLED_write(60,30,g_latitude);
  OLED_write(0,40,"Longitude: ");
  OLED_write(60,40,g_longitude);
 if(_ROLE==1)
  {
    OLED_write(0,50,"RSSI/SNR: ");
    OLED_write(60,50,String(rssi_value)+"/");
    OLED_write(80,50,String(snr_value));
  }

}

void OLED_PUB_DATA(){
    OLED_write(95,50,"PUB");

}

//LOGO only
void logo(){

   display.clear();
   display.setFont(ArialMT_Plain_10); //large font
   OLED_write(0,54, "PWD Ver 1."+_SUBVERSION+" STARTING");
   //logo at "images.h"
   display.drawXbm(0,0,PWD_width,PWD_height,PWD_bits);
    display.display();
   delay(LOGO_DELAY);
   display.clear(); 
   display.drawXbm(21,0,FRAME_width,FRAME_height,frame1_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
   display.drawXbm(21,0,FRAME_width,FRAME_height,frame2_bits);
    display.display();
   delay(FRAME_DELAY);
   display.clear(); 
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame3_bits);
     display.display();
    delay(FRAME_DELAY);
    display.clear(); 
    display.drawXbm(21,0,FRAME_width,FRAME_height,frame4_bits);
     display.display();
    delay(3*FRAME_DELAY); 
    display.clear();
}

void _OledInit(){
  writeSerial("_OledInit");
  //OLED PIN OUT CFG
  pinMode(16,OUTPUT); //OLED PIN OUT
  digitalWrite(16, LOW);    // OLED RESET
  delay(50); 
  digitalWrite(16, HIGH); // With OLED ON, GPIO16 must be HIGH
  //INIT DISPLAY
  display.init();
  display.flipScreenVertically(); 
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10); //small font
  
  delay(1500);
  //LOGO STARTUP
  logo();  


}
